/*===========================================================================*\
 | 
 |  FILE:	frame_mod_edit.cpp
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
#include "uvwframe.h"

/*===========================================================================*\
 |	ModifyObject will do all the work in a full modifier
 |  This includes casting objects to their correct form, doing modifications
 |  changing their parameters, etc
\*===========================================================================*/

void UVWFrameModifier::ModifyObject(TimeValue t, ModContext &mc, ObjectState *os, INode *node) 
{
	Interval valid = FOREVER;

	int mapCh = 0;
	int vCol = 0;
	pblock->GetValue( uvw_type, 0, vCol, valid);
	if (vCol == 0)
		pblock->GetValue( uvw_channel, 0, mapCh, FOREVER );

	INode * fr_node;
	pblock->GetValue( frame_node, 0, fr_node, FOREVER );
	
	if ( !fr_node ) 
		return;
	
	ObjectState obst = fr_node->EvalWorldState(t);
	UVWFrameObject *frame = (UVWFrameObject*)obst.obj;
	Matrix3 ftm = fr_node->GetObjTMAfterWSM(t, &valid);

	BOOL uvwData = FALSE;
	Tab <Point3> uvw_verts;
	Tab <UVWFace> uvw_faces;

	frame->GetUVW(uvwData,uvw_verts,uvw_faces,ftm,t);

	if ( os->obj->IsSubClassOf(triObjectClassID) ) {
		TriObject *tobj = (TriObject*)os->obj;

		if ( tobj->mesh.numFaces == uvw_faces.Count() ) {
		
			tobj->mesh.setMapSupport(mapCh, TRUE);
			tobj->mesh.setNumMapVerts(mapCh,uvw_verts.Count());
			tobj->mesh.setNumMapFaces(mapCh,uvw_faces.Count());

			TVFace *tvFace = tobj->mesh.mapFaces(mapCh);
			Point3 *tVerts = tobj->mesh.mapVerts(mapCh);

			for (int nv=0; nv<uvw_verts.Count(); nv++) {
				tVerts[nv] = uvw_verts[nv];
				}

			for(int nf=0; nf<uvw_faces.Count(); nf++) {
				tvFace[nf].t[0] = uvw_faces[nf].vtx[0];
				tvFace[nf].t[1] = uvw_faces[nf].vtx[1];
				tvFace[nf].t[2] = uvw_faces[nf].vtx[2];
				}
			}
		}

	else if ( os->obj->IsSubClassOf(polyObjectClassID) ) {
		PolyObject *pobj = (PolyObject*)os->obj;

		if ( pobj->GetMesh().numf == uvw_faces.Count() ) {

			for ( int i_f=0; i_f<uvw_faces.Count(); i_f++ ) {
				if ( pobj->GetMesh().f[i_f].deg != uvw_faces[i_f].deg ) {
					MessageBox(GetCOREInterface()->GetMAXHWnd(),_T("Poly Objects Mismatch"),_T("REMOVE"),MB_OK);
					return;
					}
				}

			bool hasMap;
			if (mapCh >= pobj->GetMesh().MNum()) {
				pobj->GetMesh().SetMapNum (mapCh+1);
				hasMap = false;
				}
			else {
				hasMap = !pobj->GetMesh().M(mapCh)->GetFlag (MN_DEAD);
				}
	
			MNMap *mnmap = pobj->GetMesh().M(mapCh);
			if ( mnmap->GetFlag(MN_DEAD ) )
				mnmap->ClearFlag (MN_DEAD);

			if ( !hasMap ) 
				mnmap->setNumFaces( uvw_faces.Count() );
			mnmap->setNumVerts( uvw_verts.Count() );

			for (int i_v=0; i_v<uvw_verts.Count(); i_v++) {
				mnmap->v[i_v] = uvw_verts[i_v];
				}

			for ( int i_f=0; i_f<uvw_faces.Count(); i_f++ ) {
				int face_deg = uvw_faces[i_f].deg;
				mnmap->f[i_f].SetSize( uvw_faces[i_f].deg );
				for ( int i_v=0; i_v<face_deg; i_v++ )
					mnmap->f[i_f].tv[i_v] = uvw_faces[i_f].vtx[i_v];
				}
			}
		}

	else if ( os->obj->IsSubClassOf(patchObjectClassID) ) {
		PatchObject *ptobj = (PatchObject *)os->obj;

		if ( ptobj->patch.numPatches != uvw_faces.Count() )
			return;

		ptobj->patch.setMapSupport(mapCh, TRUE);
		ptobj->patch.setNumMapVerts( mapCh, uvw_verts.Count(), TRUE );

		TVPatch *tvPatch = ptobj->patch.tvPatches[mapCh];
		PatchTVert *tVerts = ptobj->patch.tVerts[mapCh];

		for (int i_v=0; i_v<uvw_verts.Count(); i_v++) {
			tVerts[i_v] = uvw_verts[i_v];
			}

		for(int i_f=0; i_f<uvw_faces.Count(); i_f++) {
			tvPatch[i_f].tv[0] = uvw_faces[i_f].vtx[0];
			tvPatch[i_f].tv[1] = uvw_faces[i_f].vtx[1];
			tvPatch[i_f].tv[2] = uvw_faces[i_f].vtx[2];
			tvPatch[i_f].tv[3] = uvw_faces[i_f].vtx[3];
			}
		}

	// Delete the 

	// Update all the caches etc
	Interval iv = LocalValidity(t);
	iv = iv & os->obj->ChannelValidity(t,GEOM_CHAN_NUM);
	iv = iv & os->obj->ChannelValidity(t,TOPO_CHAN_NUM);
	os->obj->UpdateValidity(TEXMAP_CHAN_NUM,FOREVER);
}



/*===========================================================================*\
 |	NotifyInputChanged is called each time the input object is changed in some way
 |	We can find out how it was changed by checking partID and message
\*===========================================================================*/

void UVWFrameModifier::NotifyInputChanged(Interval changeInt, PartID partID, RefMessage message, ModContext *mc)
{

}
