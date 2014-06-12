/*===========================================================================*\
 | 
 |  FILE:	frame_mod.h
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

#ifndef __FRAME_MOD__H
#define __FRAME_MOD__H

#include "max.h"
#include "iparamm2.h"
#include "istdplug.h"
#include "meshadj.h"
#include "modstack.h"
#include "macrorec.h"
#include "resource.h"

// IMPORTANT:
// The ClassID must be changed whenever a new project
// is created using this skeleton
#define	PUREM_CLASSID		Class_ID(0x3e446f14, 0x63605fb4)

TCHAR *GetString(int id);
extern ClassDesc* GetUVWFrameModDesc();

// Paramblock2 name
enum { frame_mod_params, }; 
// Paramblock2 parameter list
enum { uvw_type, uvw_channel, frame_node };

/*===========================================================================*\
 |	UVWFrameModifier class defn
\*===========================================================================*/

class UVWFrameModifier : public Modifier{
	public:

		// Access to the interface
		static IObjParam *ip;

		// Global parameter block
		IParamBlock2	*pblock;

		//Constructor/Destructor
		UVWFrameModifier();
		~UVWFrameModifier() {}
		void DeleteThis() { delete this; }

		// Plugin identification
		void GetClassName(TSTR& s) { s= TSTR(GetString(IDS_FRAMEMOD_CLASSNAME)); }  
		virtual Class_ID ClassID() { return PUREM_CLASSID;}		
		TCHAR *GetObjectName() { return GetString(IDS_FRAMEMOD_CLASSNAME); }

		// Defines the behavior for this modifier
		// This is currently setup to be basic geometry 
		// modification of deformable objects
		ChannelMask ChannelsUsed()  { return PART_GEOM|PART_TOPO|PART_TEXMAP|PART_VERTCOLOR; }
		ChannelMask ChannelsChanged() { return PART_GEOM|PART_TOPO|PART_TEXMAP|PART_VERTCOLOR; }
		Class_ID InputType() { return defObjectClassID; }
		BOOL ChangeTopology() {return FALSE;}

		// Calculate the local validity from the parameters
		Interval LocalValidity(TimeValue t);
		Interval GetValidity(TimeValue t);

		// Object modification and notification of change
		void ModifyObject(TimeValue t, ModContext &mc, ObjectState *os, INode *node);
		void NotifyInputChanged(Interval changeInt, PartID partID, RefMessage message, ModContext *mc);

		// Reference support
		int NumRefs() { return 1; }
		RefTargetHandle GetReference(int i);
		void SetReference(int i, RefTargetHandle rtarg);

#ifndef MAX_RELEASE_R9
		RefTargetHandle Clone(RemapDir& remap = NoRemap());
#else
		RefTargetHandle Clone(RemapDir& remap = DefaultRemapDir());
#endif

		RefResult NotifyRefChanged( Interval changeInt,RefTargetHandle hTarget, 
		   PartID& partID, RefMessage message);
		
		// SubAnim support
		int NumSubs() { return 1; }
		Animatable* SubAnim(int i);
		TSTR SubAnimName(int i);


		// Direct paramblock access
		int	NumParamBlocks() { return 1; }	
		IParamBlock2* GetParamBlock(int i) { return pblock; }
		IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock->ID() == id) ? pblock : NULL; }
		int GetParamBlockIndex(int id) {return id;}

		// Does not use createmouse callbacks
		CreateMouseCallBack* GetCreateMouseCallBack() {return NULL;}

		// Load and unload our UI
		void BeginEditParams(IObjParam *ip, ULONG flags,Animatable *prev);
		void EndEditParams(IObjParam *ip, ULONG flags,Animatable *next);
		void InvalidateUI();
};


/*===========================================================================*\
 |	Dialog Processor
\*===========================================================================*/

class SkeletonPureModDlgProc : public ParamMap2UserDlgProc 
{
	public:
		UVWFrameModifier *mod;

		SkeletonPureModDlgProc() {}
		SkeletonPureModDlgProc(UVWFrameModifier *mod_in) { mod = mod_in; }

		//Win32 : BOOL DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		INT_PTR DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void DeleteThis() { }

		void SetThing(ReferenceTarget *m) {
			mod = (UVWFrameModifier*)m;
			}

};

#endif

