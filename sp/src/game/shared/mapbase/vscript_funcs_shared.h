//========= Mapbase - https://github.com/mapbase-source/source-sdk-2013 =================
//
// Purpose: See vscript_funcs_shared.cpp
//
// $NoKeywords: $
//=============================================================================

#ifndef VSCRIPT_FUNCS_SHARED
#define VSCRIPT_FUNCS_SHARED
#ifdef _WIN32
#pragma once
#endif

//-----------------------------------------------------------------------------
// Exposes csurface_t to VScript
//-----------------------------------------------------------------------------
class CSurfaceScriptAccessor
{
public:
	CSurfaceScriptAccessor( csurface_t &surf ) { m_surf = &surf; m_surfaceData = g_pScriptVM->RegisterInstance( physprops->GetSurfaceData( m_surf->surfaceProps ) ); }
	~CSurfaceScriptAccessor() { delete m_surfaceData; }

	// cplane_t stuff
	const char* Name() const { return m_surf->name; }
	HSCRIPT SurfaceProps() const { return m_surfaceData; }

	void Destroy() { delete this; }

private:
	csurface_t *m_surf;
	HSCRIPT m_surfaceData;
};

//-----------------------------------------------------------------------------
// Exposes cplane_t to VScript
//-----------------------------------------------------------------------------
class CPlaneTInstanceHelper : public IScriptInstanceHelper
{
	bool Get( void *p, const char *pszKey, ScriptVariant_t &variant )
	{
		cplane_t *pi = ((cplane_t *)p);
		if (FStrEq(pszKey, "normal"))
			variant = pi->normal;
		else if (FStrEq(pszKey, "dist"))
			variant = pi->dist;
		else
			return false;

		return true;
	}

	//bool Set( void *p, const char *pszKey, ScriptVariant_t &variant );
};

//-----------------------------------------------------------------------------
// Exposes trace_t to VScript
//-----------------------------------------------------------------------------
class CTraceInfoAccessor
{
public:
	~CTraceInfoAccessor()
	{
		if (m_surfaceAccessor)
		{
			CSurfaceScriptAccessor *pScriptSurface = HScriptToClass<CSurfaceScriptAccessor>( m_surfaceAccessor );
			//g_pScriptVM->RemoveInstance( m_surfaceAccessor );
			delete pScriptSurface;
		}

		//if (m_planeAccessor)
		//{
		//	g_pScriptVM->RemoveInstance( m_planeAccessor );
		//}
	}

	// CGrameTrace stuff
	bool DidHitWorld() const { return m_tr.DidHitWorld(); }
	bool DidHitNonWorldEntity() const { return m_tr.DidHitNonWorldEntity(); }
	int GetEntityIndex() const { return m_tr.GetEntityIndex(); }
	bool DidHit() const { return m_tr.DidHit(); }

	float FractionLeftSolid() const { return m_tr.fractionleftsolid; }
	int HitGroup() const { return m_tr.hitgroup; }
	int PhysicsBone() const { return m_tr.physicsbone; }

	HSCRIPT Entity() const { return ToHScript(m_tr.m_pEnt); }

	int HitBox() const { return m_tr.hitbox; }

	// CBaseTrace stuff
	bool IsDispSurface() { return m_tr.IsDispSurface(); }
	bool IsDispSurfaceWalkable() { return m_tr.IsDispSurfaceWalkable(); }
	bool IsDispSurfaceBuildable() { return m_tr.IsDispSurfaceBuildable(); }
	bool IsDispSurfaceProp1() { return m_tr.IsDispSurfaceProp1(); }
	bool IsDispSurfaceProp2() { return m_tr.IsDispSurfaceProp2(); }

	const Vector& StartPos() const { return m_tr.startpos; }
	const Vector& EndPos() const { return m_tr.endpos; }

	float Fraction() const { return m_tr.fraction; }

	int Contents() const { return m_tr.contents; }
	int DispFlags() const { return m_tr.dispFlags; }

	bool AllSolid() const { return m_tr.allsolid; }
	bool StartSolid() const { return m_tr.startsolid; }

	HSCRIPT Surface() { return m_surfaceAccessor; }
	void SetSurface( HSCRIPT hSurfAccessor ) { m_surfaceAccessor = hSurfAccessor; }

	HSCRIPT Plane() { return m_planeAccessor; }
	void SetPlane( HSCRIPT hPlaneAccessor ) { m_planeAccessor = hPlaneAccessor; }

	trace_t &GetTrace() { return m_tr; }
	void Destroy() { delete this; }

private:
	trace_t m_tr;

	HSCRIPT m_surfaceAccessor;
	HSCRIPT m_planeAccessor;
};

//-----------------------------------------------------------------------------
// Exposes animevent_t to VScript
//-----------------------------------------------------------------------------
class CAnimEventTInstanceHelper : public IScriptInstanceHelper
{
	bool Get( void *p, const char *pszKey, ScriptVariant_t &variant );
	bool Set( void *p, const char *pszKey, ScriptVariant_t &variant );
};

//-----------------------------------------------------------------------------
// Exposes EmitSound_t to VScript
//-----------------------------------------------------------------------------
struct ScriptEmitSound_t : public EmitSound_t
{
	int GetChannel() { return m_nChannel; }
	void SetChannel( int nChannel ) { m_nChannel = nChannel; }

	const char *GetSoundName() { return m_pSoundName; }
	void SetSoundName( const char *pSoundName ) { m_pSoundName = pSoundName; }

	float GetVolume() { return m_flVolume; }
	void SetVolume( float flVolume ) { m_flVolume = flVolume; }

	int GetSoundLevel() { return m_SoundLevel; }
	void SetSoundLevel( int iSoundLevel ) { m_SoundLevel = (soundlevel_t)iSoundLevel; }

	int GetFlags() { return m_nFlags; }
	void SetFlags( int nFlags ) { m_nFlags = nFlags; }

	int GetSpecialDSP() { return m_nSpecialDSP; }
	void SetSpecialDSP( int nSpecialDSP ) { m_nSpecialDSP = nSpecialDSP; }

	ScriptVariant_t GetOrigin() { return m_pOrigin ? *m_pOrigin : ScriptVariant_t(); }
	void SetOrigin( ScriptVariant_t origin ) { m_pOrigin = origin.m_pVector; }

	float GetSoundTime() { return m_flSoundTime; }
	void SetSoundTime( float flSoundTime ) { m_flSoundTime = flSoundTime; }

	float GetEmitCloseCaption() { return m_bEmitCloseCaption; }
	void SetEmitCloseCaption( bool bEmitCloseCaption ) { m_bEmitCloseCaption = bEmitCloseCaption; }

	float GetWarnOnMissingCloseCaption() { return m_bWarnOnMissingCloseCaption; }
	void SetWarnOnMissingCloseCaption( bool bWarnOnMissingCloseCaption ) { m_bWarnOnMissingCloseCaption = bWarnOnMissingCloseCaption; }

	float GetWarnOnDirectWaveReference() { return m_bWarnOnDirectWaveReference; }
	void SetWarnOnDirectWaveReference( bool bWarnOnDirectWaveReference ) { m_bWarnOnDirectWaveReference = bWarnOnDirectWaveReference; }

	int GetSpeakerEntity() { return m_nSpeakerEntity; }
	void SetSpeakerEntity( int nSpeakerEntity ) { m_nSpeakerEntity = nSpeakerEntity; }

	int GetSoundScriptHandle() { return m_hSoundScriptHandle; }
	void SetSoundScriptHandle( int hSoundScriptHandle ) { m_hSoundScriptHandle = hSoundScriptHandle; }
};

#endif
