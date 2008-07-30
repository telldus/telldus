Attribute VB_Name = "TellStickModule"
Public Declare Function devTurnOn Lib "TellUsbD101.dll" (ByVal lngDeviceId As Long) As Boolean
Public Declare Function devTurnOff Lib "TellUsbD101.dll" (ByVal lngDeviceId As Long) As Boolean
Public Declare Function devBell Lib "TellUsbD101.dll" (ByVal lngDeviceId As Long) As Boolean
Public Declare Function devDim Lib "TellUsbD101.dll" (ByVal lngDeviceId As Long, ByVal level As Byte) As Boolean
Public Declare Function devMethods Lib "TellUsbD101.dll" (ByVal lngDeviceId As Long) As Long

Public Declare Function devGetDeviceId Lib "TellUsbD101.dll" (ByVal a As Long) As Long
Public Declare Function devGetName Lib "TellUsbD101.dll" (ByVal i As Long) As String
Public Declare Function devGetNumberOfDevices Lib "TellUsbD101.dll" () As Long

Public Const TELLSTICK_TURNON = 1
Public Const TELLSTICK_TURNOFF = 2
Public Const TELLSTICK_BELL = 4
Public Const TELLSTICK_TOGGLE = 8
Public Const TELLSTICK_DIM = 16

