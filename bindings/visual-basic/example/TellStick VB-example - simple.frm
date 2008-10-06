VERSION 5.00
Begin VB.Form frmMain 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "TellStick VB-example - simple"
   ClientHeight    =   4680
   ClientLeft      =   2040
   ClientTop       =   3600
   ClientWidth     =   3990
   Icon            =   "TellStick VB-example - simple.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4680
   ScaleWidth      =   3990
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton cmdDim 
      Caption         =   "90%"
      Height          =   465
      Index           =   8
      Left            =   3480
      TabIndex        =   12
      Top             =   4110
      Width           =   420
   End
   Begin VB.CommandButton cmdDim 
      Caption         =   "80%"
      Height          =   465
      Index           =   7
      Left            =   3060
      TabIndex        =   11
      Top             =   4110
      Width           =   420
   End
   Begin VB.CommandButton cmdDim 
      Caption         =   "70%"
      Height          =   465
      Index           =   6
      Left            =   2640
      TabIndex        =   10
      Top             =   4110
      Width           =   420
   End
   Begin VB.CommandButton cmdDim 
      Caption         =   "60%"
      Height          =   465
      Index           =   5
      Left            =   2220
      TabIndex        =   9
      Top             =   4110
      Width           =   420
   End
   Begin VB.CommandButton cmdDim 
      Caption         =   "50%"
      Height          =   465
      Index           =   4
      Left            =   1800
      TabIndex        =   8
      Top             =   4110
      Width           =   420
   End
   Begin VB.CommandButton cmdDim 
      Caption         =   "40%"
      Height          =   465
      Index           =   3
      Left            =   1380
      TabIndex        =   7
      Top             =   4110
      Width           =   420
   End
   Begin VB.CommandButton cmdDim 
      Caption         =   "30%"
      Height          =   465
      Index           =   2
      Left            =   960
      TabIndex        =   6
      Top             =   4110
      Width           =   420
   End
   Begin VB.CommandButton cmdDim 
      Caption         =   "20%"
      Height          =   465
      Index           =   1
      Left            =   540
      TabIndex        =   5
      Top             =   4110
      Width           =   420
   End
   Begin VB.CommandButton cmdDim 
      Caption         =   "10%"
      Height          =   465
      Index           =   0
      Left            =   120
      TabIndex        =   4
      Top             =   4110
      Width           =   420
   End
   Begin VB.CommandButton cmdBell 
      Caption         =   "Bell"
      Height          =   735
      Left            =   1500
      Picture         =   "TellStick VB-example - simple.frx":0ECA
      Style           =   1  'Graphical
      TabIndex        =   3
      Top             =   3285
      Width           =   1000
   End
   Begin VB.ListBox deviceList 
      Height          =   3180
      Left            =   60
      TabIndex        =   2
      Top             =   60
      Width           =   3810
   End
   Begin VB.CommandButton cmdOff 
      Caption         =   "Off"
      Height          =   735
      Left            =   120
      Picture         =   "TellStick VB-example - simple.frx":1794
      Style           =   1  'Graphical
      TabIndex        =   1
      Top             =   3285
      Width           =   1000
   End
   Begin VB.CommandButton cmdOn 
      Caption         =   "On"
      Height          =   735
      Left            =   2900
      Picture         =   "TellStick VB-example - simple.frx":205E
      Style           =   1  'Graphical
      TabIndex        =   0
      Top             =   3285
      Width           =   1000
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdBell_Click()
    Dim blnBell As Boolean
    blnBell = devBell(deviceList.ItemData(deviceList.ListIndex))
End Sub

Private Sub cmdDim_Click(Index As Integer)
    Dim blnDim As Boolean
    Dim level As Byte
    Select Case Index
        Case "8"
            level = 230
        Case "7"
            level = 204
        Case "6"
            level = 179
        Case "5"
            level = 153
        Case "4"
            level = 128
        Case "3"
            level = 102
        Case "2"
            level = 77
        Case "1"
            level = 51
        Case "0"
            level = 25
    End Select
    blnDim = devDim(deviceList.ItemData(deviceList.ListIndex), level)
End Sub

Private Sub cmdOff_Click()
    Dim blnOff As Boolean
    blnOff = devTurnOff(deviceList.ItemData(deviceList.ListIndex))
End Sub

Private Sub cmdOn_Click()
    Dim blnOn As Boolean
    blnOn = devTurnOn(deviceList.ItemData(deviceList.ListIndex))
End Sub

Private Sub deviceList_Click()
    Dim intMethods As Integer

    intMethods = devMethods(deviceList.ItemData(deviceList.ListIndex))
    cmdOn.Enabled = (intMethods And TELLSTICK_TURNON)
    cmdOff.Enabled = (intMethods And TELLSTICK_TURNOFF)
    cmdBell.Enabled = (intMethods And TELLSTICK_BELL)

    For a = 0 To 8
      cmdDim(a).Enabled = (intMethods And TELLSTICK_DIM)
    Next

End Sub

Private Sub Form_Load()
    
    Dim numDevices As Integer, i As Integer
    numDevices = devGetNumberOfDevices()
    
    For i = 0 To numDevices - 1
        Dim strName As String
        Dim intId As Integer
        
        intId = devGetDeviceId(i)
        strName = devGetName(intId)
        deviceList.AddItem strName
        deviceList.ItemData(i) = intId
    Next

    deviceList.ListIndex = 0

End Sub
