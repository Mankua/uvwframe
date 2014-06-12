/*===========================================================================*\
 | 
 |  FILE:	Plugin.cpp
 |			Skeleton project and code for a Helper object
 |			3D Studio MAX R3.0
 | 
 |  AUTH:   Harry Denholm
 |			Developer Consulting Group
 |			Copyright(c) Discreet 1999
 |
 |  HIST:	Started 16-3-99
 | 
\*===========================================================================*/

#include "UVWFrame.h"
#include "resource.h"
#include "io.h"
#include <fcntl.h>

IObjParam*			UVWFrameObject::ip				= NULL;

#define UVWFRAME_LABEL	"U V W   F r a m e"

/*===========================================================================*\
 |	Class Descriptor
\*===========================================================================*/

class UVWFrameClassDesc:public ClassDesc2 {
	public:
	int 			IsPublic()					{ return TRUE; }
	void *			Create( BOOL loading )		{ return new UVWFrameObject; }
	const TCHAR *	ClassName()					{ return GetString(IDS_UVWFRAME_CLASSNAME); }
	SClass_ID		SuperClassID()				{ return HELPER_CLASS_ID; }
	Class_ID 		ClassID()					{ return UVWFRAME_CLASSID; }
	const TCHAR* 	Category()					{ return _T("");  }

	// Hardwired name, used by MAX Script as unique identifier
	const TCHAR*	InternalName()				{ return _T("UVWFrame"); }
	HINSTANCE		HInstance()					{ return hInstance; }
};

static UVWFrameClassDesc UVWFrameCD;
ClassDesc* GetUVWFrameDesc() {return &UVWFrameCD;}

/*===========================================================================*\
 |	Basic implimentation of a dialog handler
\*===========================================================================*/

//WIN32 : static BOOL CALLBACK AboutDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
static INT_PTR CALLBACK AboutDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_INITDIALOG:
			CenterWindow(hWnd,GetParent(hWnd));
		break;

		case WM_CLOSE:
		case WM_COMMAND:
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			EndDialog(hWnd,1);
		break;

		default:
			return FALSE;
	}
	return TRUE;
}

static HIMAGELIST hAboutImage = NULL;
static HIMAGELIST hHelpImage = NULL;

static void LoadImages()
{
	if (!hAboutImage) 
	{
		HBITMAP hBitmap, hMask;
		hAboutImage = ImageList_Create(16, 16, ILC_COLOR8|ILC_MASK, 3, 0);
		hBitmap     = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_ABOUT));
		hMask       = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_ABOUT_MASK));
		ImageList_Add(hAboutImage,hBitmap,hMask);
		DeleteObject(hBitmap);
		DeleteObject(hMask);
	}
	if (!hHelpImage) 
	{
		HBITMAP hBitmap, hMask;
		hHelpImage  = ImageList_Create(16, 16, ILC_COLOR8|ILC_MASK, 3, 0);
		hBitmap     = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_HELP));
		hMask       = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_HELP_MASK));
		ImageList_Add(hHelpImage,hBitmap,hMask);
		DeleteObject(hBitmap);
		DeleteObject(hMask);
	}
}	

//WIN32 : BOOL UVWFrameDlgProc::DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
INT_PTR UVWFrameDlgProc::DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int id = LOWORD(wParam);
	switch (msg) 
	{
		case WM_INITDIALOG:
			SetWindowText(GetDlgItem(hWnd,IDC_TKU_ABOUT),UVWFRAME_LABEL);
			if (sho->uvwMesh)
				SetWindowText(GetDlgItem(hWnd, IDC_UVWF_PICKOBJ), sho->uvwMesh->GetName()); 
			else
				SetWindowText(GetDlgItem(hWnd, IDC_UVWF_PICKOBJ), _T("Pick Object")); 

			LoadImages();
			ICustButton *iTmp;

			iTmp = GetICustButton(GetDlgItem(hWnd,IDC_TL_ABOUT));
			iTmp->SetImage(hAboutImage, 0, 0, 0, 0, 16, 16);
			iTmp->SetTooltip(TRUE,_T("About UVW Frame"));

			iTmp = GetICustButton(GetDlgItem(hWnd,IDC_TL_HELP));
			iTmp->SetImage(hHelpImage, 0, 0, 0, 0, 16, 16);
			iTmp->SetTooltip(TRUE,_T("About UVW Frame"));

			ReleaseICustButton(iTmp);
		break;

		case WM_DESTROY:
		break;

		case WM_COMMAND:
			switch(id) 
			{
				case IDC_TL_ABOUT:
					DialogBoxParam(	hInstance, MAKEINTRESOURCE(IDD_ABOUT), GetCOREInterface()->GetMAXHWnd(), AboutDlgProc, 0);
				break;

				case IDC_TL_HELP:
					ShellExecute(NULL, "open", "http://www.mankua.com/uvwframe.php", NULL, NULL, SW_SHOWNORMAL);
				break;

				case IDC_UVWF_SAVE:
					sho->SaveUVW(hWnd);
				break;

				case IDC_UVWF_LOAD:
					sho->LoadUVW(hWnd);
				break;
			}
		break;
	}

	return FALSE;
}


/*===========================================================================*\
 |	Paramblock2 Descriptor
\*===========================================================================*/
class MeshPBAccessor : public PBAccessor
{ 
public:
	void Set(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t)    // set from v
	{
		UVWFrameObject* fo = (UVWFrameObject*)owner;
		switch (id)
		{
			case pb_uvwmesh:
			{
				IParamMap2* pmap = fo->pblock2->GetMap();
				if (pmap)
					if (v.r == NULL)
						SetWindowText(GetDlgItem(pmap->GetHWnd(), IDC_UVWF_PICKOBJ), 
										GetString(IDS_UVWFRAME_NONE)); 
					else
						SetWindowText(GetDlgItem(pmap->GetHWnd(), IDC_UVWF_PICKOBJ), 
										((INode*)(v.r))->GetName()); 
				break;
			}
		}
	}
};
static MeshPBAccessor mesh_accessor;


static ParamBlockDesc2 shelp_param_blk ( shelp_params, _T("UVWFrameParams"),  0, &UVWFrameCD, P_AUTO_CONSTRUCT + P_AUTO_UI, 0, 
	//rollout
#ifdef FREE_TOOL
	IDD_UVW_FRAME_BANNER, IDS_FRAME_PARAMETERS, 0, 0, NULL, 
#else
	IDD_UVW_FRAME, IDS_FRAME_PARAMETERS, 0, 0, NULL, 
#endif
	// params

	pb_length,			_T("pb_length"),	TYPE_FLOAT,	P_ANIMATABLE,	IDS_UVF_LEN,
		p_default,		0.0f,
		p_range, 		0.0f, 65535.0f, 
		p_ui,			TYPE_SPINNER, EDITTYPE_UNIVERSE, IDC_UVWF_LEN, IDC_UVWF_LEN_SPIN, 0.1f,
		end,

	pb_width,			_T("pb_width"),	TYPE_FLOAT,	P_ANIMATABLE,	IDS_UVF_WID,
		p_default,		0.0f,
		p_range, 		0.0f, 65535.0f, 
		p_ui,			TYPE_SPINNER, EDITTYPE_UNIVERSE, IDC_UVWF_WID, IDC_UVWF_WID_SPIN, 0.1f,
		end,

	pb_height,			_T("pb_height"),TYPE_FLOAT,	P_ANIMATABLE,	IDS_UVF_HEI,
		p_default,		0.0f,
		p_range, 		0.0f, 65535.0f, 
		p_ui,			TYPE_SPINNER, EDITTYPE_UNIVERSE, IDC_UVWF_HEI, IDC_UVWF_HEI_SPIN, 0.1f,
		end,

	pb_uvwmesh, 		_T("pb_uvwmesh"), 	TYPE_INODE, 	P_OWNERS_REF + P_NO_AUTO_LABELS,	IDS_UVF_MESH,
		p_ui, 			TYPE_PICKNODEBUTTON, IDC_UVWF_PICKOBJ, 
		p_sclassID,		GEOMOBJECT_CLASS_ID, 
		p_refno,		UVW_MESH_REF,
		p_accessor,		&mesh_accessor,
		end, 

	end
	);




/*===========================================================================*\
 |	Constructor
 |  Ask the ClassDesc2 to make the AUTO_CONSTRUCT paramblocks and wire them in
\*===========================================================================*/

UVWFrameObject::UVWFrameObject()
	{
	pblock2 = NULL;
	uvwMesh = NULL;

	UVWFrameCD.MakeAutoParamBlocks(this);
	assert(pblock2);
	suspendSnap = FALSE;

	selectedInMankua = 0;
	}

UVWFrameObject::~UVWFrameObject() {
	}


/*===========================================================================*\
 |	Invalidate our UI (or the recently changed parameter)
\*===========================================================================*/

void UVWFrameObject::InvalidateUI()
{
	shelp_param_blk.InvalidateUI(pblock2->LastNotifyParamID());
}



/*===========================================================================*\
 |	Open and Close dialog UIs
 |	We ask the ClassDesc2 to handle Beginning and Ending EditParams for us
\*===========================================================================*/

void UVWFrameObject::BeginEditParams( IObjParam *ip, ULONG flags,Animatable *prev )
{
	this->ip = ip;
	assert(ip);

	if (flags&BEGIN_EDIT_CREATE)	creating = TRUE;
	else							creating = FALSE;

	UVWFrameCD.BeginEditParams(ip, this, flags, prev);
	shelp_param_blk.SetUserDlgProc(new UVWFrameDlgProc(this));

	SetNodeName();
}
		
void UVWFrameObject::EndEditParams( IObjParam *ip, ULONG flags,Animatable *next )
{
	UVWFrameCD.EndEditParams(ip, this, flags, next);
	this->ip = NULL;
}



/*===========================================================================*\
 |	Clone the helper
\*===========================================================================*/

RefTargetHandle UVWFrameObject::Clone(RemapDir& remap) {
	UVWFrameObject* newob = new UVWFrameObject();	
	newob->ReplaceReference(0,pblock2->Clone(remap));
	newob->ReplaceReference(UVW_MESH_REF,uvwMesh);
	return(newob);
	}


/*===========================================================================*\
 |	Subanim & References support
\*===========================================================================*/

Animatable* UVWFrameObject::SubAnim(int i) 	{
	switch (i) {
		case 0: return pblock2;
		default: return NULL;
		}
	}
TSTR UVWFrameObject::SubAnimName(int i) {
	switch (i) {
		case 0: return GetString(IDS_FRAME_PARAMETERS);
		default: return _T("");
		}
	}

RefTargetHandle UVWFrameObject::GetReference(int i)
	{
	switch (i) {
		case PBLOCK_REF:		return pblock2;
		case UVW_MESH_REF:		return uvwMesh;
		default: return NULL;
		}
	}
void UVWFrameObject::SetReference(int i, RefTargetHandle rtarg)
	{
	switch (i) {
		case PBLOCK_REF: 
			pblock2 = (IParamBlock2*)rtarg; 
			break;
		case UVW_MESH_REF:
			uvwMesh = (INode*)rtarg;
			break;
		}
	}
RefResult UVWFrameObject::NotifyRefChanged(
		Interval changeInt, RefTargetHandle hTarget,
		PartID& partID,  RefMessage message) 
	{
	switch (message) {
		case REFMSG_CHANGE:
			shelp_param_blk.InvalidateUI();
			break;

		case REFMSG_TARGET_DELETED:
			if (hTarget == uvwMesh) uvwMesh = NULL;
			NotifyDependents(FOREVER,PART_ALL,REFMSG_CHANGE);
			break;
		}
	return REF_SUCCEED;
	}


/*===========================================================================*\
 |	The validity of our parameters
 |	Start at FOREVER, and intersect with the validity of each item
\*===========================================================================*/

Interval UVWFrameObject::ObjectValidity(TimeValue t)
	{
	float f;	
	Interval valid = FOREVER;
	pblock2->GetValue(pb_length, t, f, valid);
	pblock2->GetValue(pb_width, t, f, valid);
	pblock2->GetValue(pb_height, t, f, valid);
	return valid;
	}

// TLTODO:	Implementar UNDO / REDO
BOOL UVWFrameObject::AddUVWMesh(INode *node)
	{
	if (node->TestForLoop(FOREVER,this)==REF_SUCCEED) {
		TimeValue t = ip->GetTime();

		ReplaceReference(UVW_MESH_REF,(RefTargetHandle)node);

		NotifyDependents(FOREVER,PART_ALL,REFMSG_CHANGE);
		SetNodeName();
		return TRUE;
	} else {
		return FALSE;
		}
	}

void UVWFrameObject::SetNodeName() {
	if (!ip) return;

	if (uvwMesh) {
		}
	else{
		}
	}

float UVWFrameObject::GetLength(TimeValue t) {
	float l;
	Interval ivalid = FOREVER;
	pblock2->GetValue(pb_length,t,l,ivalid);
	return l;
	}

float UVWFrameObject::GetWidth(TimeValue t) {
	float w;
	Interval ivalid = FOREVER;
	pblock2->GetValue(pb_width,t,w,ivalid);
	return w;
	}

float UVWFrameObject::GetHeight(TimeValue t) {
	float h;
	Interval ivalid = FOREVER;
	pblock2->GetValue(pb_height,t,h,ivalid);
	return h;
	}

void UVWFrameObject::SetLength(TimeValue t, float val) {
	pblock2->SetValue(pb_length,t,val);
	}

void UVWFrameObject::SetWidth(TimeValue t, float val) {
	pblock2->SetValue(pb_width,t,val);
	}

void UVWFrameObject::SetHeight(TimeValue t, float val) {
	pblock2->SetValue(pb_height,t,val);
	}

void UVWFrameObject::SetNode(TimeValue t,INode * node) {
	pblock2->SetValue(pb_uvwmesh,t,node);
	}	
