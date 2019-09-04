import sys
from distutils.core import setup, Extension

DEBUG = True

PLATFORM_IS_WINDOWS = sys.platform.lower().startswith('win')

if PLATFORM_IS_WINDOWS:

	__doc__="""This is a distutils setup-script for the telldus extension
	
	To build the telldus extensions, simply execute:
	  python setup.py -q build
	or
	  python setup.py -q install
	to build and install into your current Python installation.
	
	These extensions require a number of libraries to build, some of which may
	require you to install special SDKs or toolkits.  This script will attempt
	to build as many as it can, and at the end of the build will report any 
	extension modules that could not be built and why.
	
	This has got complicated due to the various different versions of
	Visual Studio used - some VS versions are not compatible with some SDK
	versions.  Below are the Windows SDK versions required (and the URL - although
	these are subject to being changed by MS at any time:)
	
	Python 2.6+:    
	
		Build using Microsoft Visual Studio 2008 Express Edition:
			http://www.microsoft.com/en-us/download/details.aspx?id=6506
			http://jenshuebel.wordpress.com/2009/02/12/visual-c-2008-express-edition-and-64-bit-targets/
			http://www.cppblog.com/Files/xcpp/VCE64BIT_WIN7SDK.zip

		For 32bit build:
			Microsoft Windows SDK for Windows 7 and .NET Framework 3.5
			http://www.microsoft.com/downloads/en/details.aspx?FamilyID=6b6c21d2-2006-4afa-9702-529fa782d63b
			Copy C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvars32.bat to
			C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat
			
		For 64bit build:
			Microsoft Windows SDK for Windows 7 and .NET Framework 3.5 SP1 (ISO) GRMSDKX_EN_DVD.iso 
			http://www.microsoft.com/en-us/download/details.aspx?id=18950     
			Copy C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvars64.bat to
			C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat
			
	Python 2.3->2.5:
		
		Microsoft Windows Software Development Kit Update for Windows Vista (version 6.0)
		http://www.microsoft.com/downloads/en/details.aspx?FamilyID=4377f86d-c913-4b5c-b87e-ef72e5b4e065
		** If you want to build Python 2.3, be sure to install the SDK compilers
		 too - although we don't use them, this setup option installs some .lib
		 files we do need.
		**
	
	If you multiple SDK versions on a single machine, set the MSSDK environment
	variable to point at the one you want to use.  Note that using the SDK for
	a particular platform (eg, Windows 7) doesn't force you to use that OS as your
	build environment.  If the links above don't work, use google to find them.
	
	Building:
	---------
	
	To install the telldus extension, execute:
	  python setup.py -q install
	
	This will install the built extensions into your site-packages directory,
	create an appropriate .pth file, and should leave everything ready to use.
	There is no need to modify the registry.
	
	To build or install debug (_d) versions of these extensions, ensure you have
	built or installed a debug version of Python itself, then pass the "--debug"
	flag to the build command - eg:
	  python setup.py -q build --debug
	or to build and install a debug version:
	  python setup.py -q build --debug install
	
	To build 64bit versions of this:
	
	* py2.5 and earlier - sorry, I've given up in disgust.  Using VS2003 with
	  the Vista SDK is just too painful to make work, and VS2005 is not used for
	  any released versions of Python. See revision 1.69 of this file for the
	  last version that attempted to support and document this process.
	
	*  2.6 and later: On a 64bit OS, just build as you would on a 32bit platform.
	   On a 32bit platform (ie, to cross-compile), you must use VS2008 to
	   cross-compile Python itself. Note that by default, the 64bit tools are not
	   installed with VS2008, so you may need to adjust your VS2008 setup (see "For 64bit build"). Then
	   use:
		  vcs
		  setup.py build --plat-name=win-amd64
	
	   see the distutils cross-compilation documentation for more details.
	"""
	# Originally by Thomas Heller, started in 2000 or so.
	import os
	import shutil
	
	is_py3k = sys.version_info > (3,) # get this out of the way early on...
	# We have special handling for _winreg so our setup3.py script can avoid
	# using the 'imports' fixer and therefore start much faster...
	if is_py3k:
		import winreg as _winreg
	else:
		import _winreg
		
	try:
		from distutils import log
	except ImportError:
		class Log:
			def debug(self, msg, *args):
				print(msg % args)
			def info(self, msg, *args):
				print(msg % args)
		log = Log()
		
	try:
		this_file = __file__
	except NameError:
		this_file = sys.argv[0]
	
	this_file = os.path.abspath(this_file)
	# We get upset if the cwd is not our source dir, but it is a PITA to
	# insist people manually CD there first!
	if os.path.dirname(this_file):
		os.chdir(os.path.dirname(this_file))

	
	# We need to know the platform SDK dir before we can list the extensions.
	def find_platform_sdk_dir():
		# Finding the Platform SDK install dir is a treat. There can be some
		# dead ends so we only consider the job done if we find the "windows.h"
		# landmark.
		landmark = "include\\windows.h"
		# 1. The use might have their current environment setup for the
		#    SDK, in which case the "MSSdk" env var is set.
		sdkdir = os.environ.get("MSSdk")
		if sdkdir:
			if DEBUG:
				print("PSDK: try %%MSSdk%%: '%s'" % sdkdir)
			if os.path.isfile(os.path.join(sdkdir, landmark)):
				return sdkdir
		# 2. The "Install Dir" value in the
		#    HKLM\Software\Microsoft\MicrosoftSDK\Directories registry key
		#    sometimes points to the right thing. However, after upgrading to
		#    the "Platform SDK for Windows Server 2003 SP1" this is dead end.
		try:
			key = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE,
								  r"Software\Microsoft\MicrosoftSDK\Directories")
			sdkdir, ignore = _winreg.QueryValueEx(key, "Install Dir")
		except EnvironmentError:
			pass
		else:
			if DEBUG:
				print(r"PSDK: try 'HKLM\Software\Microsoft\MicrosoftSDK"\
					   "\Directories\Install Dir': '%s'" % sdkdir)
			if os.path.isfile(os.path.join(sdkdir, landmark)):
				return sdkdir
		# 3. Each installed SDK (not just the platform SDK) seems to have GUID
		#    subkey of HKLM\Software\Microsoft\MicrosoftSDK\InstalledSDKs and
		#    it *looks* like the latest installed Platform SDK will be the
		#    only one with an "Install Dir" sub-value.
		try:
			key = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE,
								  r"Software\Microsoft\MicrosoftSDK\InstalledSDKs")
			i = 0
			while True:
				guid = _winreg.EnumKey(key, i)
				guidkey = _winreg.OpenKey(key, guid)
				try:
					sdkdir, ignore = _winreg.QueryValueEx(guidkey, "Install Dir")
				except EnvironmentError:
					pass
				else:
					if DEBUG:
						print(r"PSDK: try 'HKLM\Software\Microsoft\MicrosoftSDK"\
							   "\InstallSDKs\%s\Install Dir': '%s'"\
							   % (guid, sdkdir))
					if os.path.isfile(os.path.join(sdkdir, landmark)):
						return sdkdir
				i += 1
		except EnvironmentError:
			pass
		# 4.  Vista's SDK
		try:
			key = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE,
								  r"Software\Microsoft\Microsoft SDKs\Windows")
			sdkdir, ignore = _winreg.QueryValueEx(key, "CurrentInstallFolder")
		except EnvironmentError:
			pass
		else:
			if DEBUG:
				print(r"PSDK: try 'HKLM\Software\Microsoft\MicrosoftSDKs"\
					   "\Windows\CurrentInstallFolder': '%s'" % sdkdir)
			if os.path.isfile(os.path.join(sdkdir, landmark)):
				return sdkdir
	
		# 5. Failing this just try a few well-known default install locations.
		progfiles = os.environ.get("ProgramFiles", r"C:\Program Files")
		defaultlocs = [
			os.path.join(progfiles, "Microsoft Platform SDK"),
			os.path.join(progfiles, "Microsoft SDK"),
		]
		for sdkdir in defaultlocs:
			if DEBUG:
				print("PSDK: try default location: '%s'" % sdkdir)
			if os.path.isfile(os.path.join(sdkdir, landmark)):
				return sdkdir
	
	
	# Some nasty hacks to prevent most of our extensions using a manifest, as
	# the manifest - even without a reference to the CRT assembly - is enough
	# to prevent the extension from loading.  For more details, see
	# http://bugs.python.org/issue7833 - that issue has a patch, but it is
	# languishing and will probably never be fixed for Python 2.6...
	if sys.version_info > (2,6):
		from distutils.spawn import spawn
		from distutils.msvc9compiler import MSVCCompiler
		MSVCCompiler._orig_spawn = MSVCCompiler.spawn
		MSVCCompiler._orig_link = MSVCCompiler.link
	
		# We need to override this method for versions where issue7833 *has* landed
		# (ie, 2.7 and 3.2+)
		def manifest_get_embed_info(self, target_desc, ld_args):
			_want_assembly_kept = getattr(self, '_want_assembly_kept', False)
			if not _want_assembly_kept:
				return None
			for arg in ld_args:
				if arg.startswith("/MANIFESTFILE:"):
					orig_manifest = arg.split(":", 1)[1]
					if target_desc==self.EXECUTABLE:
						rid = 1
					else:
						rid = 2
					return orig_manifest, rid
			return None
		# always monkeypatch it in even though it will only be called in 2.7
		# and 3.2+.
		MSVCCompiler.manifest_get_embed_info = manifest_get_embed_info
			
		def monkeypatched_spawn(self, cmd):
			is_link = cmd[0].endswith("link.exe") or cmd[0].endswith('"link.exe"')
			is_mt = cmd[0].endswith("mt.exe") or cmd[0].endswith('"mt.exe"')
			_want_assembly_kept = getattr(self, '_want_assembly_kept', False)
			if not _want_assembly_kept and is_mt:
				# We don't want mt.exe run...
				return
			if not _want_assembly_kept and is_link:
				# remove /MANIFESTFILE:... and add MANIFEST:NO
				# (but note that for winxpgui, which specifies a manifest via a
				# .rc file, this is ignored by the linker - the manifest specified
				# in the .rc file is still added)
				for i in range(len(cmd)):
					if cmd[i].startswith("/MANIFESTFILE:"):
						cmd[i] = "/MANIFEST:NO"
						break
			if _want_assembly_kept and is_mt:
				# We want mt.exe run with the original manifest
				for i in range(len(cmd)):
					if cmd[i] == "-manifest":
						cmd[i+1] = cmd[i+1] + ".orig"
						break
			self._orig_spawn(cmd)
			if _want_assembly_kept and is_link:
				# We want a copy of the original manifest so we can use it later.
				for i in range(len(cmd)):
					if cmd[i].startswith("/MANIFESTFILE:"):
						mfname = cmd[i][14:]
						shutil.copyfile(mfname, mfname + ".orig")
						break
	
		def monkeypatched_link(self, target_desc, objects, output_filename, *args, **kw):
			# no manifests for 3.3+ 
			self._want_assembly_kept = sys.version_info < (3,3) and \
									   (os.path.basename(output_filename).startswith("PyISAPI_loader.dll") or \
										os.path.basename(output_filename).startswith("perfmondata.dll") or \
										os.path.basename(output_filename).startswith("win32ui.pyd") or \
										target_desc==self.EXECUTABLE)
			try:
				return self._orig_link(target_desc, objects, output_filename, *args, **kw)
			finally:
				delattr(self, '_want_assembly_kept')
		MSVCCompiler.spawn = monkeypatched_spawn
		MSVCCompiler.link = monkeypatched_link
	
	def find_telldus_dev_dir():
		landmark = "telldus-core.h"
		dev_dir = os.environ.get("TELLDUS_DEVDIR")
		if dev_dir:
			if DEBUG:
				print("Telldus dev dir:" % dev_dir)
			if os.path.isfile(os.path.join(dev_dir, landmark)):
				return dev_dir

		progfiles = os.environ.get("ProgramFiles", r"C:\Program Files")
		defaultlocs = [
			os.path.join(progfiles + ' (x86)', "Telldus", "Development"),
			os.path.join(progfiles, "Telldus", "Development"),
		]
		for dev_dir in defaultlocs:
			if DEBUG:
				print("Telldus dev dir: '%s'" % dev_dir)
			if os.path.isfile(os.path.join(dev_dir, landmark)):
				return dev_dir
		if DEBUG:
			print("Telldus dev dir not found, make sure dev code is installed. Or set TELLDUS_DEVDIR.")

	sdk_dir = find_platform_sdk_dir()
	sdk_include_dir = os.path.join(sdk_dir, 'Include')

	telldus_dev_dir = find_telldus_dev_dir()
	telldus_platform = os.environ.get("TELLDUS_PLATFORM", 'x86_64')
	telldus_library_dir = os.path.join(telldus_dev_dir, telldus_platform)
	
	include_dirs = [sdk_include_dir, telldus_dev_dir]
	library_dirs = [telldus_library_dir]
	libraries = ['python%i%i' % (sys.version_info[0], sys.version_info[1]) , 'TelldusCore']
	define_macros = [('_WINDOWS', 1)]
	
else:
	include_dirs = ['/usr/include', '/usr/local/include']
	library_dirs = ['/usr/lib', '/usr/local/lib']
	libraries = ['telldus-core']
	define_macros = []

define_macros.extend([('DATA_LENGTH', 20), ('CALLBACK_LENGTH', 20)])
	
telldus = Extension(
	'telldus',
	include_dirs = include_dirs,
	libraries = libraries,
	library_dirs = library_dirs,
	define_macros = define_macros,
	sources = ['telldus.c']
)

setup(
	name = 'telldus',
	version = '1.0',
	description = 'Python bindings for telldus',
	author='Oyvind Saltvik',
	author_email='oyvind.saltvik@gmail.com',
	url='http://github.com/fivethreeo/telldus/',
	ext_modules = [telldus]
)
