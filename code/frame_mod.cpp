/*===========================================================================*\
 | 
 |  FILE:	frame_mod.cpp
 |			A simple modifier that gets mapping coordinates from
 |			the UVW Frame helper object
 |			3D Studio MAX R3.0
 | 
 |  AUTH:   Diego A. Castaño
 |			Mankua
 |			Copyright(c) Mankua 2001
 |
 |  HIST:	Started 6-6-2001
 | 
\*===========================================================================*/

#include "frame_mod.h"
#include "..\..\texlay\code\texlay.h"

IObjParam* UVWFrameModifier::ip = NULL;

/*===========================================================================*\
 |	Class Descriptor OSM
\*===========================================================================*/

class UVWFrameModClassDesc:public ClassDesc2 {
	public:
	int 			IsPublic()					{ return TRUE; }
	void *			Create( BOOL loading )		{ return new UVWFrameModifier; }
	const TCHAR *	ClassName()					{ return GetString(IDS_FRAMEMOD_CLASSNAME); }
	SClass_ID		SuperClassID()				{ return OSM_CLASS_ID; }
	Class_ID 		ClassID()					{ return PUREM_CLASSID; }
	const TCHAR* 	Category()					{ return _T("");  }

	// Hardwired name, used by MAX Script as unique identifier
	const TCHAR*	InternalName()				{ return _T("SkeletonPureMod"); }
	HINSTANCE		HInstance()					{ return hInstance; }
};

static UVWFrameModClassDesc UVWFrameModCD;
ClassDesc* GetUVWFrameModDesc() {return &UVWFrameModCD;}


/*===========================================================================*\
 |	Basic implimentation of a dialog handler
\*===========================================================================*/
static HIMAGELIST hmmFAbout = NULL;
static HIMAGELIST hmmFHelp = NULL;
static void LoadImages()
{
	if (!hmmFAbout)
	{
		HBITMAP hBitmap, hMask;
		hmmFAbout	= ImageList_Create(16, 16, ILC_COLOR8|ILC_MASK, 3, 0);
		hBitmap     = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_ABOUT));
		hMask       = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_ABOUT_MASK));
		ImageList_Add(hmmFAbout,hBitmap,hMask);
		DeleteObject(hBitmap);
		DeleteObject(hMask);
	}
	if (!hmmFHelp)
	{
		HBITMAP hBitmap, hMask;
		hmmFHelp	= ImageList_Create(16, 16, ILC_COLOR8|ILC_MASK, 3, 0);
		hBitmap     = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_HELP));
		hMask       = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_HELP_MASK));
		ImageList_Add(hmmFHelp,hBitmap,hMask);
		DeleteObject(hBitmap);
		DeleteObject(hMask);
	}
}	

//Win32 : static BOOL CALLBACK AboutDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
static INT_PTR CALLBACK AboutDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
		{
			CenterWindow(hWnd,GetParent(hWnd));			
		}
		break;

		case WM_CLOSE:
			EndDialog(hWnd,1);
			break;

		default:
			return FALSE;
		}
	return TRUE;
	}

//Win32 : BOOL SkeletonPureModDlgProc::DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
INT_PTR SkeletonPureModDlgProc::DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int id = LOWORD(wParam);
	switch (msg) 
	{
		case WM_INITDIALOG: {
			LoadImages();
			// About Button
			ICustButton *iTmp;

			iTmp = GetICustButton(GetDlgItem(hWnd,IDC_UVWF_ABOUT));
			iTmp->SetImage(hmmFAbout, 0, 0, 0, 0, 16, 16);
			iTmp->SetTooltip(TRUE,_T("About UVW Frame"));

			iTmp = GetICustButton(GetDlgItem(hWnd,IDC_UVWF_HELP));
			iTmp->SetImage(hmmFHelp, 0, 0, 0, 0, 16, 16);
			iTmp->SetTooltip(TRUE,_T("UVW Frame Help"));

			ReleaseICustButton(iTmp);
			break;
			}
		case WM_DESTROY:
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam)) 
			{
				case IDC_UVWF_ABOUT:
					DialogBoxParam(	hInstance, MAKEINTRESOURCE(IDD_ABOUT), mod->ip->GetMAXHWnd(), AboutDlgProc, 0);
				break;

				case IDC_UVWF_HELP:
					ShellExecute(NULL, _T("open"), _T("http://www.mankua.com/uvwframe.php"), NULL, NULL, SW_SHOWNORMAL);
				break;
			}
			break;
	}
	return FALSE;
}


/*===========================================================================*\
 |	Paramblock2 Descriptor
\*===========================================================================*/

#if MAX_VERSION_MAJOR < 15 // Max2013
 #define p_end end
#endif

static ParamBlockDesc2 skpurem_param_blk ( frame_mod_params, _T("SkeletonPureModParams"),  0, &UVWFrameModCD, P_AUTO_CONSTRUCT + P_AUTO_UI, 0, 
	//rollout
	IDD_UVW_FRAME_MOD, IDS_FRAMEMOD_PARAMETERS, 0, 0, NULL, 

	// params
	uvw_type,			_T("uvw_type"),		TYPE_INT,	0,	IDS_SIMPLE,
		p_default,		0,
		p_range,		0,	1,
		p_ui,			TYPE_RADIO, 2, IDC_UVW_TYPE, IDC_VCC_TYPE,
		p_end,

	uvw_channel,		_T("uvw_channel"),	TYPE_INT,	0,	IDS_SIMPLE,
		p_default,		1,
		p_range, 		1, 99, 
		p_ui,			TYPE_SPINNER, EDITTYPE_INT, IDC_UVWCH_EDIT, IDC_UVWCH_SPIN, 1.0,
		p_end,

	frame_node, 		_T("uvw_frame"), 		TYPE_INODE, 	0,		IDS_SIMPLE,
		p_ui, 			TYPE_PICKNODEBUTTON, IDC_PICK_FRAME, 
		p_sclassID,		HELPER_CLASS_ID,
		p_classID,		UVWFRAME_CLASSID,
		p_end, 

	p_end
	);

/*===========================================================================*\
 |	Constructor
 |  Ask the ClassDesc2 to make the AUTO_CONSTRUCT paramblocks and wire them in
\*===========================================================================*/

UVWFrameModifier::UVWFrameModifier()
	{
	pblock = NULL;
	UVWFrameModCD.MakeAutoParamBlocks(this);
	assert(pblock);
	}



/*===========================================================================*\
 |	Invalidate our UI (or the recently changed parameter)
\*===========================================================================*/

void UVWFrameModifier::InvalidateUI()
{
	skpurem_param_blk.InvalidateUI(pblock->LastNotifyParamID());
}



/*===========================================================================*\
 |	Open and Close dialog UIs
 |	We ask the ClassDesc2 to handle Beginning and Ending EditParams for us
\*===========================================================================*/

void UVWFrameModifier::BeginEditParams( IObjParam *ip, ULONG flags,Animatable *prev )
{
	this->ip = ip;
	UVWFrameModCD.BeginEditParams(ip, this, flags, prev);
	skpurem_param_blk.SetUserDlgProc(new SkeletonPureModDlgProc(this));
}
		
void UVWFrameModifier::EndEditParams( IObjParam *ip, ULONG flags,Animatable *next )
{
	UVWFrameModCD.EndEditParams(ip, this, flags, next);
	this->ip = NULL;
}



/*===========================================================================*\
 |	Standard clone
\*===========================================================================*/


RefTargetHandle UVWFrameModifier::Clone(RemapDir& remap) 
{	
	UVWFrameModifier* newmod = new UVWFrameModifier();	
	newmod->ReplaceReference(0,pblock->Clone(remap));
	return(newmod);
}




/*===========================================================================*\
 |	Subanim & References support
\*===========================================================================*/

Animatable* UVWFrameModifier::SubAnim(int i)
{
	switch (i)
	{
		case 0: return pblock;
		default: return NULL;
	}
}

TSTR UVWFrameModifier::SubAnimName(int i) 
{
	switch (i)
	{
		case 0: return GetString(IDS_FRAMEMOD_PARAMETERS);
		default: return _T("");
	}
}

RefTargetHandle UVWFrameModifier::GetReference(int i)
	{
	switch (i) {
		case 0: return pblock;
		default: return NULL;
		}
	}

void UVWFrameModifier::SetReference(int i, RefTargetHandle rtarg)
	{
	switch (i) {
		case 0: pblock = (IParamBlock2*)rtarg; break;
		}
	}

#if MAX_VERSION_MAJOR < 17 //Max 2015
RefResult UVWFrameModifier::NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, 
   PartID& partID, RefMessage message ) 
#else
RefResult UVWFrameModifier::NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, 
   PartID& partID, RefMessage message, BOOL propagate ) 
#endif
	{
	switch (message) {
		case REFMSG_CHANGE:
			skpurem_param_blk.InvalidateUI();
			break;
		}
	return REF_SUCCEED;
	}




/*===========================================================================*\
 |	The validity of our parameters
 |	Start at FOREVER, and intersect with the validity of each item
\*===========================================================================*/

Interval UVWFrameModifier::GetValidity(TimeValue t)
{
	float f;	
	Interval valid = FOREVER;
	pblock->GetValue(uvw_channel, t, f, valid);
	return valid;
}

Interval UVWFrameModifier::LocalValidity(TimeValue t)
{
	return GetValidity(t);
}
