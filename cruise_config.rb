Project.configure do |project|
	project.email_notifier.emails = ["micke.prag@telldus.se"]
	project.email_notifier.from = 'micke.prag@telldus.se'
	project.build_command = './cruise_build.sh'
end
