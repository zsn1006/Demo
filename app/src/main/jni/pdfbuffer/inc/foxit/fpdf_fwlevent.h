/** Copyright (C) 2012 Foxit Corporation. All Rights Reserved.
  * The following code is copyrighted and contains proprietary information and trade secrets of Foxit Corporation. 
  * You can only redistribute files listed below to customers of your application, under a written SDK license agreement with Foxit. 
  * You cannot distribute any part of the SDK to general public, even with a SDK license agreement. 
  * Without SDK license agreement, you cannot redistribute anything.
  * @file	fpdf_fwlevent.h
  * @author	Foxit Corporation
  * @brief	Extended header file for the Form Field module.<br>
  *			The content of this header file is an extension of the form field module.
  *			In this headfile, we define external event messages(including messages of mouse and keyboard), 
  *			which are defined by this SDK and independent of platforms. 
  *			Applications should send data to methods of this SDK in correct message formats according to definitions in this file.
  * @note	If you want to purchase Foxit PDF SDK license and use ANY of the following functions, please
  *			request for enabling Form Field module explicitly. 
  * @version 2.0
  */

/** 
 * @addtogroup FPDFFORMFIELD Form Field
 * @brief Definitions and Methods in this module are included in fpdf_formfield.h and fpdf_fwlevent.h .
 */
/**@{*/

#ifndef _FWL_EVENT_H_
#define _FWL_EVENT_H_

#ifndef _FS_BASE_H_
#include "fs_base.h"
#endif

#ifndef _FS_DEF_FWLEVENTTYPE_
#define _FS_DEF_FWLEVENTTYPE_
/** @brief Enumeration for Event Types. */
typedef enum _FWL_EVENTTYPE_
{
	FWL_EVENTTYPE_Mouse = 0	,	/**< @brief The type of the event is mouse. */
	FWL_EVENTTYPE_MouseWheel,	/**< @brief The type of the event is mousewheel. */
	FWL_EVENTTYPE_Key		,	/**< @brief The type of the event is key. */
} FWL_EVENTTYPE;
#endif

#ifndef _FS_DEF_FWLEVENTFLAG_
#define _FS_DEF_FWLEVENTFLAG_
/** @brief Enumeration for Key Flags. */
typedef enum  _FWL_EVENTFLAG_
{    
	FWL_EVENTFLAG_ShiftKey			= 1 << 0,	/**< @brief The flag of the event: the "Shift" key. */
	FWL_EVENTFLAG_ControlKey		= 1 << 1,	/**< @brief The flag of the event: the "Ctrl" key. */
    FWL_EVENTFLAG_AltKey			= 1 << 2,	/**< @brief The flag of the event: the "Alt" key. */
    FWL_EVENTFLAG_MetaKey			= 1 << 3,	/**< @brief The flag of the event: meta key. */
    FWL_EVENTFLAG_KeyPad			= 1 << 4,	/**< @brief The flag of the event: key pad. */
    FWL_EVENTFLAG_AutoRepeat		= 1 << 5,	/**< @brief The flag of the event: auto repeat. */
    FWL_EVENTFLAG_LeftButtonDown	= 1 << 6,	/**< @brief The flag of the event: the left button of the mouse is down. */
    FWL_EVENTFLAG_MiddleButtonDown	= 1 << 7,	/**< @brief The flag of the event: the middle button of the mouse is down. */
    FWL_EVENTFLAG_RightButtonDown	= 1 << 8,	/**< @brief The flag of the event: the right button of the mouse is down. */
} FWL_EVENTFLAG; 
#endif 

#ifndef _FS_DEF_FWLEVENTMOUSECMD_
#define _FS_DEF_FWLEVENTMOUSECMD_
/** @brief Enumeration for Mouse Message Commands*/
typedef enum _FWL_EVENT_MOUSECMD_
{
	FWL_EVENTMOUSECMD_LButtonDown =	1	,	/**< @brief The mouse command event: the left button of the mouse is down. */
	FWL_EVENTMOUSECMD_LButtonUp			,	/**< @brief The mouse command event: the left button of the mouse is up. */
	FWL_EVENTMOUSECMD_LButtonDblClk		,	/**< @brief The mouse command event: the left button of the mouse is double clicked. */
	FWL_EVENTMOUSECMD_RButtonDown		,	/**< @brief The mouse command event: the right button of the mouse is down. */
	FWL_EVENTMOUSECMD_RButtonUp			,	/**< @brief The mouse command event: the right button of the mouse is up. */
	FWL_EVENTMOUSECMD_RButtonDblClk		,	/**< @brief The mouse command event: the right button of the mouse is double clicked. */
	FWL_EVENTMOUSECMD_MButtonDown		,	/**< @brief The mouse command event: the middle button of the mouse is down. */
	FWL_EVENTMOUSECMD_MButtonUp			,	/**< @brief The mouse command event: the middle button of the mouse is up. */
	FWL_EVENTMOUSECMD_MButtonDblClk		,	/**< @brief The mouse command event: the middle button of the mouse is double clicked. */
	FWL_EVENTMOUSECMD_MouseMove			,	/**< @brief The mouse command event: mouse move. */
	FWL_EVENTMOUSECMD_MouseEnter		,	/**< @brief The mouse command event: mouse enter. */
	FWL_EVENTMOUSECMD_MouseHover		,	/**< @brief The mouse command event: mouse hover. */
	FWL_EVENTMOUSECMD_MouseLeave		,	/**< @brief The mouse command event: mouse leave. */
} FWL_EVENT_MOUSECMD;
#endif

#ifndef _FS_DEF_EVENTMOUSE_
#define _FS_DEF_EVENTMOUSE_
/** @brief Structure for Mouse Event. */
typedef struct _FWL_EVENT_MOUSE_
{
	FS_DWORD command;	/**< @brief The mouse command. See ::FWL_EVENT_MOUSECMD. */
	FS_DWORD flag;		/**< @brief The event flag. See ::FWL_EVENTFLAG. */
	FS_FLOAT x;			/**< @brief The value of the x-coordinate of the mouse position. */
	FS_FLOAT y;			/**< @brief The value of the y-coordinate of the mouse position. */
}FWL_EVENT_MOUSE;
#endif

#ifndef _FS_DEF_EVENTMOUSEWHEEL_
#define _FS_DEF_EVENTMOUSEWHEEL_
/** @brief Structure for Mouse Wheel. */
typedef struct _FWL_EVENT_MOUSEWHEEL_
{	
	FS_DWORD flag;		/**< @brief The event flag. See ::FWL_EVENTFLAG. */
	FS_FLOAT x;			/**< @brief The value of the x-coordinate of the mouse wheel position. */
	FS_FLOAT y;			/**< @brief The value of the y-coordinate of the mouse wheel position. */
	FS_FLOAT deltaX;	/**< @brief The delta value in the x-coordinate direction. */
    FS_FLOAT deltaY;	/**< @brief The delta value in the y-coordinate direction. */
}FWL_EVENT_MOUSEWHEEL;	
#endif

#ifndef _FS_DEF_FWLVKEYCODE_
#define _FS_DEF_FWLVKEYCODE_
/** @brief Enumeration for Virtual Key Codes */
typedef enum _FWL_VKEYCODE_
{
  FWL_VKEY_Back			= 0x08,	/**< @brief  Backspace key. */
  FWL_VKEY_Tab			= 0x09,	/**< @brief  Tab key. */
  FWL_VKEY_Clear		= 0x0C,	/**< @brief  Clear key. */
  FWL_VKEY_Return		= 0x0D,	/**< @brief  Enter key. */
  FWL_VKEY_Shift		= 0x10,	/**< @brief  Shift key. */
  FWL_VKEY_Control		= 0x11,	/**< @brief  Ctrl key. */
  FWL_VKEY_Menu			= 0x12,	/**< @brief  Menu key. */
  FWL_VKEY_Pause		= 0x13,	/**< @brief  Pause Break key. */
  FWL_VKEY_Capital		= 0x14,	/**< @brief  Caps Lock key. */
  FWL_VKEY_Kana			= 0x15,	/**< @brief Used with IME. */
  FWL_VKEY_Hangul		= 0x15,	/**< @brief Used with IME. */
  FWL_VKEY_Junja		= 0x17,	/**< @brief Used with IME. */
  FWL_VKEY_Final		= 0x18,	/**< @brief Used with IME. */
  FWL_VKEY_Hanja		= 0x19,	/**< @brief Used with IME. */
  FWL_VKEY_Kanji		= 0x19,	/**< @brief Used with IME. */
  FWL_VKEY_Escape		= 0x1B,	/**< @brief  ESC key. */
  FWL_VKEY_Convert		= 0x1C,	/**< @brief Used with IME. */
  FWL_VKEY_NonConvert	= 0x1D,	/**< @brief Used with IME. */
  FWL_VKEY_Accept		= 0x1E,	/**< @brief Used with IME. */
  FWL_VKEY_ModeChange	= 0x1F,	/**< @brief Used with IME. */
  FWL_VKEY_Space		= 0x20,	/**< @brief  Space key. */
  FWL_VKEY_Prior		= 0x21,	/**< @brief  Page Up key. */
  FWL_VKEY_Next			= 0x22,	/**< @brief  Page Down key. */
  FWL_VKEY_End			= 0x23,	/**< @brief  End key. */
  FWL_VKEY_Home			= 0x24,	/**< @brief  Home key. */
  FWL_VKEY_Left			= 0x25,	/**< @brief  left arrow key. */
  FWL_VKEY_Up			= 0x26,	/**< @brief  arrow key. */
  FWL_VKEY_Right		= 0x27,	/**< @brief  right arrow key. */
  FWL_VKEY_Down			= 0x28,	/**< @brief  down arrow key. */
  FWL_VKEY_Select		= 0x29,	/**< @brief  Select key.*/
  FWL_VKEY_Print		= 0x2A,	/**< @brief  Print key. */
  FWL_VKEY_Execute		= 0x2B,	/**< @brief  Execute key. */
  FWL_VKEY_Snapshot		= 0x2C,	/**< @brief  Print Screen key. */
  FWL_VKEY_Insert		= 0x2D,	/**< @brief  Insert key. */
  FWL_VKEY_Delete		= 0x2E,	/**< @brief  Delete key. */
  FWL_VKEY_Help			= 0x2F,	/**< @brief  Help key. */
  FWL_VKEY_0 = 0x30,			/**< @brief  0 key. */
  FWL_VKEY_1 = 0x31,			/**< @brief  1 key. */
  FWL_VKEY_2 = 0x32,			/**< @brief  2 key. */
  FWL_VKEY_3 = 0x33,			/**< @brief  3 key. */
  FWL_VKEY_4 = 0x34,			/**< @brief  4 key. */
  FWL_VKEY_5 = 0x35,			/**< @brief  5 key. */
  FWL_VKEY_6 = 0x36,			/**< @brief  6 key. */
  FWL_VKEY_7 = 0x37,			/**< @brief  7 key. */
  FWL_VKEY_8 = 0x38,			/**< @brief  8 key. */
  FWL_VKEY_9 = 0x39,			/**< @brief  9 key. */
  FWL_VKEY_A = 0x41,			/**< @brief  A key. */
  FWL_VKEY_B = 0x42,			/**< @brief  B key. */
  FWL_VKEY_C = 0x43,			/**< @brief  C key. */
  FWL_VKEY_D = 0x44,			/**< @brief  D key. */
  FWL_VKEY_E = 0x45,			/**< @brief  E key. */
  FWL_VKEY_F = 0x46,			/**< @brief  F key. */
  FWL_VKEY_G = 0x47,			/**< @brief  G key. */
  FWL_VKEY_H = 0x48,			/**< @brief  H key. */
  FWL_VKEY_I = 0x49,			/**< @brief  I key. */
  FWL_VKEY_J = 0x4A,			/**< @brief  J key. */
  FWL_VKEY_K = 0x4B,			/**< @brief  K key. */
  FWL_VKEY_L = 0x4C,			/**< @brief  L key. */
  FWL_VKEY_M = 0x4D,			/**< @brief  M key. */
  FWL_VKEY_N = 0x4E,			/**< @brief  N key. */
  FWL_VKEY_O = 0x4F,			/**< @brief  O key. */
  FWL_VKEY_P = 0x50,			/**< @brief  P key. */
  FWL_VKEY_Q = 0x51,			/**< @brief  Q key. */
  FWL_VKEY_R = 0x52,			/**< @brief  R key. */
  FWL_VKEY_S = 0x53,			/**< @brief  S key. */
  FWL_VKEY_T = 0x54,			/**< @brief  T key. */
  FWL_VKEY_U = 0x55,			/**< @brief  U key. */
  FWL_VKEY_V = 0x56,			/**< @brief  V key. */
  FWL_VKEY_W = 0x57,			/**< @brief  W key. */
  FWL_VKEY_X = 0x58,			/**< @brief  X key. */
  FWL_VKEY_Y = 0x59,			/**< @brief  Y key. */
  FWL_VKEY_Z = 0x5A,			/**< @brief  Z key. */
  FWL_VKEY_LWin		= 0x5B,		/**< @brief  left windows key(Microsoft keyboard). */
  FWL_VKEY_Command	= 0x5B,		/**< @brief  command key.*/
  FWL_VKEY_RWin		= 0x5C,		/**< @brief  right windows key(Microsoft keyboard). */
  FWL_VKEY_Apps		= 0x5D,		/**< @brief  applications key(Microsoft keyboard). */
  FWL_VKEY_Sleep	= 0x5F,		/**< @brief  sleeping key. */
  FWL_VKEY_NumPad0	= 0x60,		/**< @brief  0 key in the numeric keypad. */
  FWL_VKEY_NumPad1	= 0x61,		/**< @brief  1 key in the numeric keypad. */
  FWL_VKEY_NumPad2	= 0x62,		/**< @brief  2 key in the numeric keypad. */
  FWL_VKEY_NumPad3	= 0x63,		/**< @brief  3 key in the numeric keypad. */
  FWL_VKEY_NumPad4	= 0x64,		/**< @brief  4 key in the numeric keypad. */
  FWL_VKEY_NumPad5	= 0x65,		/**< @brief  5 key in the numeric keypad. */
  FWL_VKEY_NumPad6	= 0x66,		/**< @brief  6 key in the numeric keypad. */
  FWL_VKEY_NumPad7	= 0x67,		/**< @brief  7 key in the numeric keypad. */
  FWL_VKEY_NumPad8	= 0x68,		/**< @brief  8 key in the numeric keypad. */
  FWL_VKEY_NumPad9	= 0x69,		/**< @brief  9 key in the numeric keypad. */
  FWL_VKEY_Multiply = 0x6A,		/**< @brief  * key in the numeric keypad. */
  FWL_VKEY_Add		= 0x6B,		/**< @brief  + key in the numeric keypad. */
  FWL_VKEY_Separator = 0x6C,	/**< @brief  Enter key in the numeric keypad. */
  FWL_VKEY_Subtract	= 0x6D,		/**< @brief  - key in the numeric keypad. */
  FWL_VKEY_Decimal	= 0x6E,		/**< @brief  . key in the numeric keypad. */
  FWL_VKEY_Divide	= 0x6F,		/**< @brief  / key in the numeric keypad. */
  FWL_VKEY_F1	= 0x70,			/**< @brief  F1 key. */
  FWL_VKEY_F2	= 0x71,			/**< @brief  F2 key. */
  FWL_VKEY_F3	= 0x72,			/**< @brief  F3 key. */
  FWL_VKEY_F4	= 0x73,			/**< @brief  F4 key. */
  FWL_VKEY_F5	= 0x74,			/**< @brief  F5 key. */
  FWL_VKEY_F6	= 0x75,			/**< @brief  F6 key. */
  FWL_VKEY_F7	= 0x76,			/**< @brief  F7 key. */
  FWL_VKEY_F8	= 0x77,			/**< @brief  F8 key. */
  FWL_VKEY_F9	= 0x78,			/**< @brief  F9 key. */
  FWL_VKEY_F10	= 0x79,			/**< @brief  F10 key. */
  FWL_VKEY_F11	= 0x7A,			/**< @brief  F11 key. */
  FWL_VKEY_F12	= 0x7B,			/**< @brief  F12 key. */
  FWL_VKEY_F13	= 0x7C,			/**< @brief  F13 key. */
  FWL_VKEY_F14	= 0x7D,			/**< @brief  F14 key. */
  FWL_VKEY_F15	= 0x7E,			/**< @brief  F15 key. */
  FWL_VKEY_F16	= 0x7F,			/**< @brief  F16 key. */
  FWL_VKEY_F17	= 0x80,			/**< @brief  F17 key. */
  FWL_VKEY_F18	= 0x81,			/**< @brief  F18 key. */
  FWL_VKEY_F19	= 0x82,			/**< @brief  F19 key. */
  FWL_VKEY_F20	= 0x83,			/**< @brief  F20 key. */
  FWL_VKEY_F21	= 0x84,			/**< @brief  F21 key. */
  FWL_VKEY_F22	= 0x85,			/**< @brief  F22 key. */
  FWL_VKEY_F23	= 0x86,			/**< @brief  F23 key. */
  FWL_VKEY_F24	= 0x87,			/**< @brief  F24 key. */
  FWL_VKEY_NunLock	= 0x90,		/**< @brief  Num Lock key. */
  FWL_VKEY_Scroll	= 0x91,		/**< @brief  Scroll Lock key. */
  FWL_VKEY_LShift	= 0xA0,		/**< @brief  left Shift key. */	
  FWL_VKEY_RShift	= 0xA1,		/**< @brief  right Shift key. */
  FWL_VKEY_LControl	= 0xA2,		/**< @brief  left Ctrl key. */
  FWL_VKEY_RControl	= 0xA3,		/**< @brief  right Ctrl key. */
  FWL_VKEY_LMenu	= 0xA4,		/**< @brief  left Alt key. */
  FWL_VKEY_RMenu	= 0xA5,		/**< @brief  right Alt key. */
  FWL_VKEY_BROWSER_Back		= 0xA6,		/**< @brief browser back key. */
  FWL_VKEY_BROWSER_Forward	= 0xA7,		/**< @brief browser forward key. */
  FWL_VKEY_BROWSER_Refresh	= 0xA8,		/**< @brief browser refresh key. */
  FWL_VKEY_BROWSER_Stop		= 0xA9,		/**< @brief browser stop key. */
  FWL_VKEY_BROWSER_Search	= 0xAA,		/**< @brief browser search key. */
  FWL_VKEY_BROWSER_Favorites = 0xAB,	/**< @brief browser favorites key. */
  FWL_VKEY_BROWSER_Home		= 0xAC,		/**< @brief browser home key. */
  FWL_VKEY_VOLUME_Mute		= 0xAD,		/**< @brief volume mute key. */
  FWL_VKEY_VOLUME_Down		= 0xAE,		/**< @brief volume down key. */
  FWL_VKEY_VOLUME_Up		= 0xAF,		/**< @brief volume up key. */
  FWL_VKEY_MEDIA_NEXT_Track	= 0xB0,		/**< @brief next track key. */
  FWL_VKEY_MEDIA_PREV_Track	= 0xB1,		/**< @brief previous track key. */
  FWL_VKEY_MEDIA_Stop		= 0xB2,		/**< @brief stop media key. */
  FWL_VKEY_MEDIA_PLAY_Pause = 0xB3,		/**< @brief play/pause media key. */
  FWL_VKEY_MEDIA_LAUNCH_Mail = 0xB4,	/**< @brief [media] launch mail key. */
  FWL_VKEY_MEDIA_LAUNCH_MEDIA_Select = 0xB5,	/**< @brief [media] launch media select key. */
  FWL_VKEY_MEDIA_LAUNCH_APP1 = 0xB6,	/**< @brief [media] launch APP1 key. */
  FWL_VKEY_MEDIA_LAUNCH_APP2 = 0xB7,	/**< @brief [media] launch APP2 key. */
  FWL_VKEY_OEM_1		= 0xBA,	/**< @brief ; : key. */
  FWL_VKEY_OEM_Plus		= 0xBB,	/**< @brief = + key. */
  FWL_VKEY_OEM_Comma	= 0xBC,	/**< @brief , < key. */
  FWL_VKEY_OEM_Minus	= 0xBD,	/**< @brief _ key. */
  FWL_VKEY_OEM_Period	= 0xBE,	/**< @brief . > key. */
  FWL_VKEY_OEM_2 = 0xBF,		/**< @brief / ? key. */
  FWL_VKEY_OEM_3 = 0xC0,		/**< @brief ` ~ key. */
  FWL_VKEY_OEM_4 = 0xDB,		/**< @brief [ { key. */
  FWL_VKEY_OEM_5 = 0xDC,		/**< @brief \ | key. */
  FWL_VKEY_OEM_6 = 0xDD,		/**< @brief ] } key. */
  FWL_VKEY_OEM_7 = 0xDE,		/**< @brief ' " key. */
  FWL_VKEY_OEM_8 = 0xDF,		/**< @brief Used for miscellaneous characters; it can vary by keyboard. */
  FWL_VKEY_OEM_102	= 0xE2,		/**< @brief Either the angle bracket key or the backslash key on the RT 102-key keyboard */
  FWL_VKEY_ProcessKey = 0xE5,	/**< @brief IME PROCESS key */
  FWL_VKEY_Packet	= 0xE7,		/**< @brief Used to pass Unicode characters as if they were keystrokes. */
  FWL_VKEY_Attn		= 0xF6,		/**< @brief Attn key. */
  FWL_VKEY_Crsel	= 0xF7,		/**< @brief CrSel key. */
  FWL_VKEY_Exsel	= 0xF8,		/**< @brief ExSel key. */
  FWL_VKEY_Ereof	= 0xF9,		/**< @brief Erase EOF key. */
  FWL_VKEY_Play		= 0xFA,		/**< @brief Play key. */
  FWL_VKEY_Zoom		= 0xFB,		/**< @brief Zoom key. */
  FWL_VKEY_NoName	= 0xFC,		/**< @brief The key without a name. */
  FWL_VKEY_PA1		= 0xFD,		/**< @brief PA1 key. */
  FWL_VKEY_OEM_Clear = 0xFE,	/**< @brief Clear key */
  FWL_VKEY_Unknown	= 0,		/**< @brief An unkown key. */
} FWL_VKEYCODE;
#endif

#ifndef _FS_DEF_FWLEVENTKEYCMD_
#define _FS_DEF_FWLEVENTKEYCMD_
/** @brief Enumeration for Key Event Command*/
typedef enum _FWL_EVENTKEYCMD_
{
	FWL_EVENTKEYCMD_KeyDown = 1	,	/**< @brief The key command event: key down. */
	FWL_EVENTKEYCMD_KeyUp		,	/**< @brief The key command event: key up. */
	FWL_EVENTKEYCMD_Char		,	/**< @brief The key command event: char. */
} FWL_EVENTKEYCMD;
#endif

#ifndef _FS_DEF_EVENTKEY_
#define _FS_DEF_EVENTKEY_
/** @brief Structure for Key Event */
typedef struct _FWL_EVENT_KEY_
{
	FS_DWORD command;	/** @brief The key command. See ::FWL_EVENTKEYCMD . */
	FS_DWORD flag;		/**< @brief The event flag. See ::FWL_EVENTFLAG. */
	/** @brief Union for Code */
	union
	{
		FS_DWORD vkcode;	/**< @brief Virtual key code. See ::FWL_VKEYCODE . */
		FS_DWORD charcode;	/**< @brief Character code. */
	}code;
}FWL_EVENT_KEY;
#endif

#ifndef _FS_DEF_EVENT_
#define _FS_DEF_EVENT_
/** @brief Structure for Event*/
typedef struct _FWL_EVENT_
{
	FS_DWORD size; /**< @brief The structure size. */
    FS_DWORD type; /**< @brief The event type. See ::FWL_EVENTTYPE . */
	/** @brief Union s*/
	union
	{
		FWL_EVENT_MOUSE mouse;		/**< @brief A mouse event object. */
		FWL_EVENT_MOUSEWHEEL wheel;	/**< @brief A mouse wheel event object. */
		FWL_EVENT_KEY key;			/**< @brief A key event object. */
	}s;
}FWL_EVENT; 
#endif 

#endif
// _FWL_EVENT_H
/**@}*/



