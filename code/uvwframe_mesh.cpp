/*===========================================================================*\
 | 
 |  FILE:	Plugin.cpp
 |			Skeleton project and code for a Helper object
 |			3D Studio MAX R3.0
 | 
 |  AUTH:   Diego Andres Castaño Alt
 |			Copyright(c) Mankua 2000
 |
 |  HIST:	Started 20-12-99
 | 
\*===========================================================================*/

#include "UVWFrame.h"
#include "resource.h"

/*===========================================================================*\
 |	Mouse creation 
\*===========================================================================*/

int UVWFrameCreateCallBack::proc(ViewExp *vpt, int msg, int point, int flags, IPoint2 m, Matrix3& mat ) {
	float l,w,h;
	Point3 p1, d, center;

	if (msg == MOUSE_FREEMOVE)
	{
		vpt->SnapPreview(m, m, NULL, SNAP_IN_3D);
	}


	if (msg==MOUSE_POINT||msg==MOUSE_MOVE) {
		switch(point) {

		case 0:
			sho->suspendSnap = TRUE;				
			sp0 = m;
			p0 = vpt->SnapPoint(m,m,NULL,SNAP_IN_3D);
			p1 = p0 + Point3(.01,.01,.01);
			mat.SetTrans(float(.5)*(p0+p1));				
			{
			Point3 xyz = mat.GetTrans();
			xyz.z = p0.z;
			mat.SetTrans(xyz);
			}
			break;

		case 1:
			sp1 = m;
			mat.IdentityMatrix();
			p1 = vpt->SnapPoint(m,m,NULL,SNAP_IN_3D);
			p1.z = p0.z +(float).01; 

			w = fabs(p1.x-p0.x);
			l = fabs(p1.y-p0.y);
			h = 0.0f;
			mat.SetTrans(float(.5)*(p0+p1));

			sho->pblock2->SetValue(pb_length, 0, l);
			sho->pblock2->SetValue(pb_width, 0, w);
			sho->pblock2->SetValue(pb_height, 0, h);
			if (msg==MOUSE_POINT) {
				sho->suspendSnap = FALSE;
				return CREATE_STOP;
				}
			break;

		case 2:
			p1.z = p0.z + vpt->SnapLength(vpt->GetCPDisp(p0,Point3(0,0,1),sp1,m,TRUE));
			d = p1-p0;
			
			sho->pblock2->SetValue(pb_height, 0, d.z/2.0f);
			if (msg==MOUSE_POINT) {
				sho->suspendSnap = FALSE;
				return CREATE_STOP;

			}
			break;					   
		}
	} else {
		if (msg == MOUSE_ABORT) return CREATE_ABORT;
	}

	return TRUE;
}

static UVWFrameCreateCallBack HelperCreateCB;

CreateMouseCallBack* UVWFrameObject::GetCreateMouseCallBack() 
{
	HelperCreateCB.SetObj(this);
	return(&HelperCreateCB);
}

void UVWFrameObject::SetExtendedDisplay(int flags)
{
	extDispFlags = flags;
}




/*===========================================================================*\
 |	Bounding Box calculation
\*===========================================================================*/

void UVWFrameObject::GetLocalBoundBox(
		TimeValue t, INode* inode, ViewExp* vpt, Box3& box ) 
	{
	float l,w,h; Interval ivalid = FOREVER;	
	pblock2->GetValue(pb_length,t,l,ivalid);
	pblock2->GetValue(pb_width,t,w,ivalid);
	pblock2->GetValue(pb_height,t,h,ivalid);

	box = Box3(Point3((float)-w,(float)-l,(float)-h), Point3((float)w,(float)l,(float)h));
	}

void UVWFrameObject::GetWorldBoundBox(
		TimeValue t, INode* inode, ViewExp* vpt, Box3& box )
	{
	Matrix3 tm;
	tm = inode->GetObjectTM(t);
	float l,w,h; Interval ivalid = FOREVER;	
	pblock2->GetValue(pb_length,t,l,ivalid);
	pblock2->GetValue(pb_width,t,w,ivalid);
	pblock2->GetValue(pb_height,t,h,ivalid);

	Point3 lo = tm.GetTrans();
	Point3 hi = tm.GetTrans();

//	lo.x -= w/2.0f; lo.y -= l/2.0f; lo.z -= h/2.0f;
//	hi.x += w/2.0f; hi.y += l* 0.7f; hi.z += h/2.0f;
	 
	box = Box3(lo, hi);

	box += tm * Point3(w/2.0f,l/2.0f,0.0f);
	box += tm * Point3(w/2.0f,l*0.7f,h);
	box += tm * Point3(-w/2.0f,l/2.0f,0.0f);
	box += tm * Point3(-w/2.0f,l*0.7f,h);
	box += tm * Point3(w/2.0f,-l/2.0f,0.0f);
	box += tm * Point3(w/2.0f,-l*0.7f,h);
	box += tm * Point3(-w/2.0f,-l/2.0f,0.0f);
	box += tm * Point3(-w/2.0f,-l*0.7f,h);
	}



/*===========================================================================*\
 |	Hit Testing and Object Snapping 
\*===========================================================================*/
int UVWFrameObject::HitTest(TimeValue t, INode *inode, int type, int crossing, int flags, IPoint2 *p, ViewExp *vpt) {
	Matrix3 tm(1);	
	HitRegion hitRegion;
	DWORD	savedLimits;
	float l,w,h; Interval ivalid = FOREVER;	
	pblock2->GetValue(pb_length,t,l,ivalid);
	pblock2->GetValue(pb_width,t,w,ivalid);
	pblock2->GetValue(pb_height,t,h,ivalid);

	GraphicsWindow *gw = vpt->getGW();	
	Material *mtl = gw->getMaterial();

   	tm = inode->GetObjectTM(t);		
	MakeHitRegion(hitRegion, type, crossing, 4, p);

	gw->setRndLimits(((savedLimits = gw->getRndLimits())|GW_PICK)&~GW_ILLUM);
	gw->setHitRegion(&hitRegion);
	gw->clearHitCode();

	vpt->getGW()->setTransform(tm);

	DrawBox(t,inode,vpt,TRUE);

	if((hitRegion.type != POINT_RGN) && !hitRegion.crossing)
		return TRUE;
	return gw->checkHitCode();
	}

void UVWFrameObject::Snap(TimeValue t, INode* inode, SnapInfo *snap, IPoint2 *p, ViewExp *vpt)
	{
	if(suspendSnap)
		return;
	}

/*===========================================================================*\
 |	Display the actual helper
\*===========================================================================*/

void UVWFrameObject::SelectInMankua( int sel ) {
	selectedInMankua = sel;
	NotifyDependents(FOREVER,PART_ALL,REFMSG_CHANGE);
	}

int UVWFrameObject::Display(TimeValue t, INode* inode, ViewExp *vpt, int flags) {
	Matrix3 tm(1);
	tm = inode->GetObjectTM(t);	
	vpt->getGW()->setTransform(tm);

	DrawBox(t,inode,vpt,FALSE);
/*
	int frameColor = COLOR_GIZMOS;
	Point3 tilelColor(0.35f,0.59f,0.0f);

	if ( selectedInMankua || inode->Selected() ) {
		frameColor = COLOR_SEL_GIZMOS;
		tilelColor = Point3(0.0f,0.8f,0.0f);
		}

	float l,w,h; Interval ivalid = FOREVER;	
	pblock2->GetValue(pb_length,t,l,ivalid);
	pblock2->GetValue(pb_width,t,w,ivalid);
	pblock2->GetValue(pb_height,t,h,ivalid);

	Point3	pt1((float)w/2.0f  ,(float)-l/2.0f ,0.0f), 
			pt2((float)-w/2.0f ,(float)-l/2.0f ,0.0f), 
			pt3((float)-w/2.0f ,(float)l/2.0f  ,0.0f), 
			pt4((float)w/2.0f  ,(float)l/2.0f  ,0.0f);

	Point3 pt[5] = { pt1, pt2, pt3, pt4, pt1};
	Point3 qt[2];

	qt[0] = Point3(0.0f,(float)l/2.0f,0.0f);
	qt[1] = Point3(0.0f,(float)l*0.7f,0.0f);

	if(!inode->IsFrozen())
		vpt->getGW()->setColor(LINE_COLOR, GetUIColor(frameColor) );

	vpt->getGW()->polyline( 4, pt, NULL, NULL, 0, NULL );
	vpt->getGW()->polyline( 2, qt, NULL, NULL, 0, NULL );

	qt[0] = pt[2];
	qt[1] = pt[2] + Point3(0.0f,0.0f,h);
	vpt->getGW()->polyline( 2, qt, NULL, NULL, 0, NULL );

	qt[0] = pt[1];
	qt[1] = pt[1] + Point3(0.0f,0.0f,h);
	vpt->getGW()->polyline( 2, qt, NULL, NULL, 0, NULL );

	if(!inode->IsFrozen())
		vpt->getGW()->setColor(LINE_COLOR,tilelColor);
	vpt->getGW()->polyline( 2, &pt[3], NULL, NULL, 0, NULL );

	qt[0] = pt[0];
	qt[1] = pt[0] + Point3(0.0f,0.0f,h);
	vpt->getGW()->polyline( 2, qt, NULL, NULL, 0, NULL );

	qt[0] = pt[3];
	qt[1] = pt[3] + Point3(0.0f,0.0f,h);
	vpt->getGW()->polyline( 2, qt, NULL, NULL, 0, NULL );

	for (int i=0;i<5; i++)
		pt[i].z += h;

	if(!inode->IsFrozen())
		vpt->getGW()->setColor(LINE_COLOR, GetUIColor(frameColor) );
	vpt->getGW()->polyline( 4, pt, NULL, NULL, 0, NULL );

	if(!inode->IsFrozen())
		vpt->getGW()->setColor(LINE_COLOR,tilelColor);
	vpt->getGW()->polyline( 2, &pt[3], NULL, NULL, 0, NULL );
*/
	return(0);
}

void UVWFrameObject::DrawBox(TimeValue t, INode *inode, ViewExp *vpt, BOOL hit_testing) {
	int frameColor = COLOR_GIZMOS;
	Point3 tilelColor(0.35f,0.59f,0.0f);

	if ( selectedInMankua || inode->Selected() ) {
		frameColor = COLOR_SEL_GIZMOS;
		tilelColor = Point3(0.0f,0.8f,0.0f);
		}

	float l,w,h; Interval ivalid = FOREVER;	
	pblock2->GetValue(pb_length,t,l,ivalid);
	pblock2->GetValue(pb_width,t,w,ivalid);
	pblock2->GetValue(pb_height,t,h,ivalid);

	Point3	pt1((float)w/2.0f  ,(float)-l/2.0f ,0.0f), 
			pt2((float)-w/2.0f ,(float)-l/2.0f ,0.0f), 
			pt3((float)-w/2.0f ,(float)l/2.0f  ,0.0f), 
			pt4((float)w/2.0f  ,(float)l/2.0f  ,0.0f);

	Point3 pt[5] = { pt1, pt2, pt3, pt4, pt1};
	Point3 qt[2];

	qt[0] = Point3(0.0f,(float)l/2.0f,0.0f);
	qt[1] = Point3(0.0f,(float)l*0.7f,0.0f);

	if(!inode->IsFrozen())
		vpt->getGW()->setColor(LINE_COLOR, GetUIColor(frameColor) );

	vpt->getGW()->polyline( 4, pt, NULL, NULL, 0, NULL );
	vpt->getGW()->polyline( 2, qt, NULL, NULL, 0, NULL );

	qt[0] = pt[2];
	qt[1] = pt[2] + Point3(0.0f,0.0f,h);
	vpt->getGW()->polyline( 2, qt, NULL, NULL, 0, NULL );

	qt[0] = pt[1];
	qt[1] = pt[1] + Point3(0.0f,0.0f,h);
	vpt->getGW()->polyline( 2, qt, NULL, NULL, 0, NULL );

	if(!inode->IsFrozen())
		vpt->getGW()->setColor(LINE_COLOR,tilelColor);
	vpt->getGW()->polyline( 2, &pt[3], NULL, NULL, 0, NULL );

	qt[0] = pt[0];
	qt[1] = pt[0] + Point3(0.0f,0.0f,h);
	vpt->getGW()->polyline( 2, qt, NULL, NULL, 0, NULL );

	qt[0] = pt[3];
	qt[1] = pt[3] + Point3(0.0f,0.0f,h);
	vpt->getGW()->polyline( 2, qt, NULL, NULL, 0, NULL );

	for (int i=0;i<5; i++)
		pt[i].z += h;

	if(!inode->IsFrozen())
		vpt->getGW()->setColor(LINE_COLOR, GetUIColor(frameColor) );
	vpt->getGW()->polyline( 4, pt, NULL, NULL, 0, NULL );

	if(!inode->IsFrozen())
		vpt->getGW()->setColor(LINE_COLOR,tilelColor);
	vpt->getGW()->polyline( 2, &pt[3], NULL, NULL, 0, NULL );
	}

ObjectState UVWFrameObject::Eval(TimeValue time)
{
	return ObjectState(this);
}

#if MAX_VERSION_MAJOR < 4
class UVW_TVFaceClass {
	public:
		int t[4];
		int FaceIndex;
		int MatID;
		int flags;
		Point3 pt[4];
	};

class UVW_TVVertClass {
	public:
		Point3 p;
		float influence;
		int flags;
	};
#else
class UVW_TVFaceClass {
	public:
		int count;
		int * t;
		int FaceIndex;
		int MatID;
		int flags;
		int * v;
	};

class UVW_TVVertClass {
	public:
		Point3 p;
		float influence;
		int flags;
	};
#endif

void UVWFrameObject::SaveUVW(HWND hWnd) {
	if (!uvwMesh) return;

	if (ip->GetSelNodeCount() != 1) {
		MessageBox(hWnd,_T("Please select just one UVW Frame"),_T("UVW Frame"),MB_OK);
		return;
		}

	static TCHAR fname[256] = {'\0'};
	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(ofn));
	FilterList fl;
	fl.Append( _T("UVW File"));
	fl.Append( _T("*.uvw"));		
	TSTR title = _T("Save UVW");

	ofn.lStructSize     = sizeof(OPENFILENAME);
	ofn.hwndOwner       = hWnd;
	ofn.lpstrFilter     = fl;
	ofn.lpstrFile       = fname;
	ofn.nMaxFile        = 256;    
	ofn.lpstrInitialDir = ip->GetDir(APP_EXPORT_DIR);
	ofn.Flags           = OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt     = _T("uvw");
	ofn.lpstrTitle      = title;

	tryAgain:
	if (GetSaveFileName(&ofn)) {
		if (DoesFileExist(fname)) {
			TSTR buf1;
			TSTR buf2 = _T("Save UVW");
			buf1.printf(_T("%s already exists.\nDo you want to replace it?"),fname);
			if (IDYES!=MessageBox(
				hWnd,
				buf1,buf2,MB_YESNO|MB_ICONQUESTION)) {
				goto tryAgain;
				}
			}

#if MAX_VERSION_MAJOR < 15
	FILE *file = fopen(fname,_T("wb"));
#else
	FILE *file = _tfopen(fname,_T("wb"));
#endif

	TimeValue t = ip->GetTime();
	Interval iv = FOREVER;

	Matrix3 ftm = ip->GetSelNode(0)->GetObjTMAfterWSM(t, &iv);

	float l,w,h;
	Interval ivalid = FOREVER;
	pblock2->GetValue(pb_length,t,l,ivalid);
	pblock2->GetValue(pb_width,t,w,ivalid);
	pblock2->GetValue(pb_height,t,h,ivalid);

	BOOL uvwData;
	Tab <Point3> uvw_verts;
	Tab <UVWFace> uvw_faces;
	GetUVW(uvwData,uvw_verts,uvw_faces,ftm,t);

	Tab<UVW_TVVertClass> v;
	Tab<UVW_TVFaceClass> f;

	int num_faces = uvw_faces.Count();
	int num_verts = uvw_verts.Count();

	v.SetCount( num_verts );
	for ( int i_v=0; i_v<num_verts; i_v++ ) {
		v[i_v].p = uvw_verts[i_v];
		v[i_v].influence = 0.0f;
		v[i_v].flags = 0;
		}

	f.SetCount( num_faces );
	for ( int i_f=0; i_f<num_faces; i_f++ ) {
		f[i_f].count = uvw_faces[i_f].deg;
		f[i_f].t = new int[uvw_faces[i_f].deg];
		f[i_f].v = new int[uvw_faces[i_f].deg];
		for ( int i_v=0; i_v<uvw_faces[i_f].deg; i_v++ ) {
			f[i_f].t[i_v] = uvw_faces[i_f].vtx[i_v];
			f[i_f].v[i_v] = 0;
			}

		f[i_f].FaceIndex = i_f;
		f[i_f].MatID = uvw_faces[i_f].mat_id;
		f[i_f].flags = 0;
		}

	int ver = -1;
	fwrite(&ver,sizeof(ver),1,file);
	ver = 4;
	fwrite(&ver,sizeof(ver),1,file);

	fwrite(&num_verts, sizeof(num_verts), 1,file);

	if ( num_verts ) {
		fwrite(v.Addr(0), sizeof(UVW_TVVertClass)*num_verts, 1,file);
		}
	
	fwrite(&num_faces, sizeof(num_faces), 1,file);

	if ( num_faces ) {
		for ( int i_f=0; i_f<num_faces; i_f++ ) {
			int count = f[i_f].count;
			fwrite(&f[i_f].count, sizeof(int), 1,file);
			fwrite(f[i_f].t, sizeof(int)*count, 1,file);
			fwrite(&f[i_f].FaceIndex, sizeof(int), 1,file);
			fwrite(&f[i_f].MatID, sizeof(int), 1,file);
			fwrite(&f[i_f].flags, sizeof(int), 1,file);
			fwrite(f[i_f].v, sizeof(int)*count, 1,file);
			}
		}

	fclose(file);

	for ( int i_f=0; i_f<num_faces; i_f++ ) {
		f[i_f].count = 0;
		delete [] f[i_f].t;
		delete [] f[i_f].v;
		}
	f.SetCount(0);
	v.SetCount(0);
	}
}

void UVWFrameObject::LoadUVW(HWND hWnd) {
	if (ip->GetSelNodeCount() != 1) {
		MessageBox(hWnd,_T("Please select just one UVW Frame"),_T("UVW Frame"),MB_OK);
		return;
		}
	TimeValue t = ip->GetTime();

	int i_f, i_v;

	static TCHAR fname[256] = {'\0'};
	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(ofn));
	FilterList fl;
	fl.Append( _T("UVW File"));
	fl.Append( _T("*.uvw"));		
	TSTR title = _T("Load UVW");

	ofn.lStructSize     = sizeof(OPENFILENAME);
	ofn.hwndOwner       = hWnd;
	ofn.lpstrFilter     = fl;
	ofn.lpstrFile       = fname;
	ofn.nMaxFile        = 256;    
	ofn.lpstrInitialDir = ip->GetDir(APP_EXPORT_DIR);
	ofn.Flags           = OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt     = _T("uvw");
	ofn.lpstrTitle      = title;

	if(!GetOpenFileName(&ofn)) return;

	Tab<UVW_TVVertClass> v;
	Tab<UVW_TVFaceClass> f;

	int num_faces;
	int num_verts;

	BOOL is_poly = FALSE;

#if MAX_VERSION_MAJOR < 15
	FILE *file = fopen(fname,_T("rb"));
#else
	FILE *file = _tfopen(fname,_T("rb"));
#endif

	int ver;
	fread(&ver,sizeof(ver),1,file);
	fread(&ver,sizeof(ver),1,file);

	fread(&num_verts,sizeof(num_verts),1,file);
	v.SetCount(num_verts);
	if (num_verts) 
		fread(v.Addr(0), sizeof(UVW_TVVertClass)*num_verts, 1,file);

	fread(&num_faces,sizeof(num_faces),1,file);
	f.SetCount(num_faces);
	if (num_faces) {
		for ( i_f=0; i_f<num_faces; i_f++ ) {
			fread(&f[i_f].count, sizeof(int), 1,file);
			int count = f[i_f].count;
			f[i_f].v = new int[count];
			f[i_f].t = new int[count];
			fread(f[i_f].t, sizeof(int)*count, 1,file);
			fread(&f[i_f].FaceIndex, sizeof(int), 1,file);
			fread(&f[i_f].MatID, sizeof(int), 1,file);
			fread(&f[i_f].flags, sizeof(int), 1,file);
			fread(f[i_f].v, sizeof(int)*count, 1,file);
			if ( count != 3 )
				is_poly = TRUE;
			}
		}

	fclose(file);

	float w = GetWidth( ip->GetTime() );
	float l = GetLength( ip->GetTime() );
	float h = GetHeight( ip->GetTime() );
	Point3 wlh(w,l,h);

	Matrix3 tm = ip->GetSelNode(0)->GetNodeTM(ip->GetTime());

	if ( is_poly ) {
		PolyObject *poly_obj;
		poly_obj = CreateEditablePolyObject();

		poly_obj->mm.setNumVerts(num_verts);
		poly_obj->mm.setNumFaces(num_faces);

		for( i_v=0; i_v<num_verts; i_v++ ) {
			Point3 vert = v[i_v].p * wlh - wlh/2.0f;
			vert.z = v[i_v].p.z * wlh.z;
			poly_obj->mm.v[i_v].p = vert;
			}

		for( i_f=0; i_f<num_faces; i_f++) {
			poly_obj->mm.f[i_f].MakePoly(f[i_f].count,f[i_f].t);
			poly_obj->mm.f[i_f].smGroup = (1<<0);
			poly_obj->mm.f[i_f].material = f[i_f].MatID;
			}

		poly_obj->mm.InvalidateGeomCache();
		poly_obj->mm.InvalidateTopoCache();
		poly_obj->mm.FillInMesh();
		poly_obj->mm.PrepForPipeline();

		INode *newNode = ip->CreateObjectNode(poly_obj);
		newNode->SetNodeTM(t,tm);

		SetNode(t,newNode);
		}
	else {
		TriObject *newObj;
		newObj = CreateNewTriObject();

		newObj->mesh.setNumVerts(num_verts);
		newObj->mesh.setNumFaces(num_faces);

		for( i_v=0; i_v<num_verts; i_v++ ) {
			Point3 vert = v[i_v].p * wlh - wlh/2.0f;
			vert.z = v[i_v].p.z * wlh.z;
			newObj->mesh.verts[i_v] = vert;
			}

		for( i_f=0; i_f<num_faces; i_f++) {
			newObj->mesh.faces[i_f].setVerts( f[i_f].t[0],f[i_f].t[1],f[i_f].t[2]);
			newObj->mesh.faces[i_f].flags = EDGE_A|EDGE_B|EDGE_C;
			newObj->mesh.faces[i_f].smGroup = (1<<0);
			newObj->mesh.faces[i_f].setMatID(f[i_f].MatID);
			}

		INode *newNode = ip->CreateObjectNode(newObj);
		newNode->SetNodeTM(t,tm);

		SetNode(t,newNode);
		}

	for ( i_f=0; i_f<num_faces; i_f++ ) {
		f[i_f].count = 0;
		delete [] f[i_f].t;
		delete [] f[i_f].v;
		}
	f.SetCount(0);
	v.SetCount(0);
	}

void UVWFrameObject::GetMatID(Tab <int> &matids, TimeValue t) {
	if (!uvwMesh) return;
	BitArray faceDel;
	int im = 0;
	// Recojo la informacion del Modifier Ignore
	Object* iobj;
	iobj = uvwMesh->GetObjectRef();
	if (!iobj) return;

	ObjectState ios = uvwMesh->EvalWorldState(t);
	if (ios.obj && ios.obj->SuperClassID() != GEOMOBJECT_CLASS_ID) return;

	ObjectState os = uvwMesh->EvalWorldState(t);
	TriObject *obj = NULL;
	BOOL needsDel = FALSE;

	if (os.obj->IsSubClassOf(triObjectClassID)) {
		obj = (TriObject*)os.obj;
		needsDel = FALSE;
		} 
	else
		if (os.obj->CanConvertToType(triObjectClassID)) {
			Object *oldObj = os.obj;
			obj = (TriObject*)os.obj->ConvertToType(t,triObjectClassID);
			needsDel = (obj != oldObj);
			}

	Interval iv = FOREVER;

	assert(obj);
	if (!obj) return;

	int nf = obj->GetMesh().numFaces;

	matids.SetCount(nf);

	int fcter = 0;
	for (int fct=0; fct<nf; fct++) {
		matids[fct] = obj->GetMesh().faces[fct].getMatID();
		}

	if (needsDel) obj->DeleteThis();
	}

void UVWFrameObject::GetUVW( BOOL &uvwData, Tab <Point3> &uvw_verts, Tab <UVWFace> &uvw_faces, Matrix3 ftm, TimeValue t ) {
	if (!uvwMesh) return;
	BitArray faceDel;
	int im = 0;
	// Recojo la informacion del Modifier Ignore
	Object* iobj;
	iobj = uvwMesh->GetObjectRef();
	if (!iobj) return;

	ObjectState ios = uvwMesh->EvalWorldState(t);
	if (ios.obj && ios.obj->SuperClassID() != GEOMOBJECT_CLASS_ID) return;

	ObjectState os = uvwMesh->EvalWorldState(t);

	float l,w,h;
	Interval ivalid = FOREVER;
	pblock2->GetValue(pb_length,t,l,ivalid);
	pblock2->GetValue(pb_width,t,w,ivalid);
	pblock2->GetValue(pb_height,t,h,ivalid);

	Interval iv = FOREVER;
	Matrix3 obj_tm = uvwMesh->GetObjTMAfterWSM(t, &iv);

	if ( os.obj->IsSubClassOf(polyObjectClassID) ) {
		PolyObject *pobj = (PolyObject*)os.obj;

		int num_uvw_verts = pobj->GetMesh().numv;
		int num_uvw_faces = pobj->GetMesh().numf;

		uvw_verts.SetCount(num_uvw_verts);
		uvw_faces.SetCount(num_uvw_faces);

		for ( int i_v=0; i_v<num_uvw_verts; i_v++ ) {
			Point3 p = pobj->GetMesh().v[i_v].p * obj_tm;
			p = p * Inverse(ftm);
			uvw_verts[i_v] = Point3( 0.5f + p.x / w, 0.5f + p.y / l, 0.0f );
			if ( fabs(h)>0.0001f ) 
				uvw_verts[i_v].z = p.z / h;
			}

		for ( int i_f=0; i_f<num_uvw_faces; i_f++ ) {
			int poly_deg = pobj->GetMesh().f[i_f].deg;
			uvw_faces[i_f].deg = poly_deg;
			uvw_faces[i_f].vtx = new int[poly_deg];
			uvw_faces[i_f].mat_id = (int)pobj->GetMesh().f[i_f].material;

			for ( int i_v=0; i_v<poly_deg; i_v++ ) {
				uvw_faces[i_f].vtx[i_v] = pobj->GetMesh().f[i_f].vtx[i_v];
				}
			}
		}

	else if (os.obj->IsSubClassOf(patchObjectClassID)) {
		PatchObject *ptobj = (PatchObject *)os.obj;

		int num_uvw_verts = ptobj->patch.numVerts;
		int num_uvw_faces = ptobj->patch.numPatches;

		uvw_verts.SetCount(num_uvw_verts);
		uvw_faces.SetCount(num_uvw_faces);

		for ( int i_v=0; i_v<num_uvw_verts; i_v++ ) {
			Point3 p = ptobj->patch.verts[i_v].p * obj_tm;
			p = p * Inverse(ftm);
			uvw_verts[i_v] = Point3( 0.5f + p.x / w, 0.5f + p.y / l, 0.0f );
			if ( fabs(h)>0.0001f ) 
				uvw_verts[i_v].z = p.z / h;
			}

		for ( int i_f=0; i_f<num_uvw_faces; i_f++ ) {
			uvw_faces[i_f].deg = 4;
			uvw_faces[i_f].vtx = new int[4];
			uvw_faces[i_f].mat_id = (int)ptobj->patch.patches[i_f].getMatID();

			uvw_faces[i_f].vtx[0] = ptobj->patch.patches[i_f].v[0];
			uvw_faces[i_f].vtx[1] = ptobj->patch.patches[i_f].v[1];
			uvw_faces[i_f].vtx[2] = ptobj->patch.patches[i_f].v[2];
			uvw_faces[i_f].vtx[3] = ptobj->patch.patches[i_f].v[3];
			}
		}

	else {
		TriObject *tobj = NULL;
		BOOL needsDel = FALSE;

		if (os.obj->IsSubClassOf(triObjectClassID)) {
			tobj = (TriObject*)os.obj;
			needsDel = FALSE;
			} 
		else
			if (os.obj->CanConvertToType(triObjectClassID)) {
				Object *oldObj = os.obj;
				tobj = (TriObject*)os.obj->ConvertToType(t,triObjectClassID);
				needsDel = (tobj != oldObj);
				}

		if (!tobj) 
			return;


		int num_uvw_verts = tobj->GetMesh().numVerts;
		int num_uvw_faces = tobj->GetMesh().numFaces;

		uvw_verts.SetCount(num_uvw_verts);
		uvw_faces.SetCount(num_uvw_faces);

		for ( int i_v=0; i_v<num_uvw_verts; i_v++ ) {
			Point3 p = tobj->GetMesh().verts[i_v] * obj_tm;
			p = p * Inverse(ftm);
			uvw_verts[i_v] = Point3( 0.5f + p.x / w, 0.5f + p.y / l, 0.0f );
			if ( fabs(h)>0.0001f ) 
				uvw_verts[i_v].z = p.z / h;
			}

		for ( int i_f=0; i_f<num_uvw_faces; i_f++ ) {
			uvw_faces[i_f].deg = 3;
			uvw_faces[i_f].vtx = new int[3];
			uvw_faces[i_f].mat_id = (int)tobj->GetMesh().faces[i_f].getMatID();

			uvw_faces[i_f].vtx[0] = tobj->GetMesh().faces[i_f].v[0];
			uvw_faces[i_f].vtx[1] = tobj->GetMesh().faces[i_f].v[1];
			uvw_faces[i_f].vtx[2] = tobj->GetMesh().faces[i_f].v[2];
			}

		if (needsDel) 
			tobj->DeleteThis();
		}
	}
