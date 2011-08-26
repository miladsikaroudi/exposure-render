#pragma once

#include "MonteCarlo.h"

class CCamera;

class FIDELITY_RENDER_DLL CCameraSample
{
public:
	Vec2f m_ImageXY;			/*!< Image sample position */
	Vec2f m_LensUV;				/*!< Lens sample position */
	float m_Time;				/*!< Time */

	DEV CCameraSample(void)
	{
		m_ImageXY	= Vec2f(0.0f);
		m_LensUV	= Vec2f(0.0f);
		m_Time		= 0.0f;
	}

	DEV CCameraSample& CCameraSample::operator=(const CCameraSample& Other)
	{
		m_ImageXY	= Other.m_ImageXY;
		m_LensUV	= Other.m_LensUV;
		m_Time		= Other.m_Time;

		return *this;
	}

	DEV void LargeStep(CCudaRNG& Rnd, const int& X, const int& Y, const int& KernelSize)
	{
		m_ImageXY	= StratifiedSample2D(X, Y, Rnd.Get2(), KernelSize, KernelSize);
		m_LensUV	= Rnd.Get2();//StratifiedSample2D(X, Y, Rnd.Get2(), KernelSize, KernelSize);
		m_Time		= Rnd.Get1();
	}

	DEV void Mutate(CCudaRNG& Rnd, const float& Width, const float& Height)
	{
		MutateMetro(Rnd, m_ImageXY.x);
		MutateMetro(Rnd, m_ImageXY.y);
		MutateMetro(Rnd, m_LensUV.x);
		MutateMetro(Rnd, m_LensUV.y);
		MutateMetro(Rnd, m_Time);
	}
};

class FIDELITY_RENDER_DLL CFocus
{
public:
	enum EType
	{
		CenterScreen,
		ScreenPoint,
		Probed,
		Manual
	};

	EType		m_Type;					/*!< Focus type e.g. center screen, screen point, probed and manual */
	Vec2f		m_SensorPosCanvas;		/*!< Position on canvas*/
	float		m_FocalDistance;		/*!< Focal distance */
	float		m_T;					/*!< Hit distance */
	Vec3f		m_P;					/*!< Probe position */
	Vec3f		m_N;					/*!< Probe normal */
	float		m_DotWN;				/*!< Projection */

	// ToDo: Add description
	HOD CFocus(void)
	{
		m_Type				= DEF_FOCUS_TYPE;
		m_SensorPosCanvas	= DEF_FOCUS_SENSOR_POS_CANVAS;
		m_FocalDistance		= DEF_FOCUS_FOCAL_DISTANCE;
		m_T					= DEF_FOCUS_T;
		m_P					= DEF_FOCUS_P;
		m_N					= DEF_FOCUS_N;
		m_DotWN				= DEF_FOCUS_DOT_WN;
	}

	// ToDo: Add description
	HOD ~CFocus(void)
	{
	}

	// ToDo: Add description
	HOD CFocus& CFocus::operator=(const CFocus& Other)
	{
		m_Type				= Other.m_Type;
		m_SensorPosCanvas	= Other.m_SensorPosCanvas;
		m_FocalDistance		= Other.m_FocalDistance;
		m_P					= Other.m_P;
		m_T					= Other.m_T;
		m_N					= Other.m_N;
		m_DotWN				= Other.m_DotWN;

		return *this;
	}
};

class FIDELITY_RENDER_DLL CAperture
{
public:
	enum EBias
	{
		BiasCenter,
		BiasEdge,
		BiasNone
	};

	float			m_Size;						/*!< Size */
	int				m_NoBlades;					/*!< Number of aperture blades */
	EBias			m_Bias;						/*!< Bias e.g. center, edge and none */
	float			m_Rotation;					/*!< Rotation */
	float			m_Data[MAX_BOKEH_DATA];		/*!< Data */

	// ToDo: Add description
	HOD CAperture(void)
	{
		m_Size		= DEF_APERTURE_SIZE;
		m_NoBlades	= DEF_APERTURE_NO_BLADES;
		m_Bias		= DEF_APERTURE_BIAS;
		m_Rotation	= DEF_APERTURE_ROTATION;

		for (int i = 0; i < MAX_BOKEH_DATA; i++)
			m_Data[i] = 0.0f;
	}

	// ToDo: Add description
	HOD ~CAperture(void)
	{
	}

	// ToDo: Add description
	HOD CAperture& CAperture::operator=(const CAperture& Other)
	{
		m_Size		= Other.m_Size;
		m_NoBlades	= Other.m_NoBlades;
		m_Bias		= Other.m_Bias;
		m_Rotation	= Other.m_Rotation;

		for (int i = 0; i < MAX_BOKEH_DATA; i++)
			m_Data[i] = Other.m_Data[i];

		return *this;
	}

	// ToDo: Add description
	HOD void Update(const float& FStop)
	{
		// Update bokeh
		int Ns = (int)m_NoBlades;
		
		if ((Ns >= 3) && (Ns <= 6))
		{
			float w = m_Rotation * PI_F / 180.0f, wi = (2.0f * PI_F) / (float)Ns;
		
			Ns = (Ns + 2) * 2;

			for (int i = 0; i < Ns; i += 2)
			{
				m_Data[i]		= cos(w);
				m_Data[i + 1]	= sin(w);
				w += wi;
			}
		}
	}
};

class FIDELITY_RENDER_DLL CBloom
{
public:
	float	m_Radius;			/*!< Nominal bloom radius */
	float	m_Weight;			/*!< Bloom weight */
	int		m_NoSamples;		/*!< No. samples */

	// ToDo: Add description
	HOD CBloom(void)
	{
		m_Radius		= 100.0f;
		m_Weight		= 0.1f;
		m_NoSamples		= 12;
	}

	// ToDo: Add description
	HOD ~CBloom(void)
	{
	}

	// ToDo: Add description
	HOD CBloom& CBloom::operator=(const CBloom& Other)
	{
		m_Radius		= Other.m_Radius;
		m_Weight		= Other.m_Weight;
		m_NoSamples		= Other.m_NoSamples;

		return *this;
	}

	// ToDo: Add description
	HOD void Update(void)
	{
	}
};

class FIDELITY_RENDER_DLL CToneMap
{
public:
	float	m_Factor;		/*!< Factor */
		
	// ToDo: Add description
	HOD CToneMap(void)
	{
		m_Factor = 1.0f;
	}

	// ToDo: Add description
	HOD ~CToneMap(void)
	{
	}

	// ToDo: Add description
	HOD CToneMap& CToneMap::operator=(const CToneMap& Other)
	{
		m_Factor = Other.m_Factor;
		
		return *this;
	}

	// ToDo: Add description
	HOD void Update(void)
	{
	}
};

class FIDELITY_RENDER_DLL CFilm
{
public:
	CResolution2D		m_Resolution;		/*!< Resolution */
	float				m_Screen[2][2];		/*!< Screen */
	Vec2f				m_InvScreen;		/*!< Screen reciprocal */
	float				m_Iso;				/*!< Sensitivity */
	float				m_Exposure;			/*!< Exposure */
	float				m_FStop;			/*!< F-Stop */
	float				m_Gamma;			/*!< Gamma */
	CToneMap			m_ToneMap;			/*!< Tone map */
	CBloom				m_Bloom;			/*!< Bloom */

	// ToDo: Add description
	HOD CFilm(void)
	{
		m_Screen[0][0]	= 0.0f;
		m_Screen[0][1]	= 0.0f;
		m_Screen[1][0]	= 0.0f;
		m_Screen[1][1]	= 0.0f;
		m_InvScreen		= Vec2f(0.0f);
		m_Iso			= 400.0f;
		m_Exposure		= 10.0f;
		m_FStop			= 8.0f;
		m_Gamma			= 2.2f;
	}

	// ToDo: Add description
	HOD ~CFilm(void)
	{
	}

	// ToDo: Add description
	HOD CFilm& CFilm::operator=(const CFilm& Other)
	{
		m_Resolution		= Other.m_Resolution;
		m_Screen[0][0]		= Other.m_Screen[0][0];
		m_Screen[0][1]		= Other.m_Screen[0][1];
		m_Screen[1][0]		= Other.m_Screen[1][0];
		m_Screen[1][1]		= Other.m_Screen[1][1];
		m_InvScreen			= Other.m_InvScreen;
		m_Iso				= Other.m_Iso;
		m_Exposure			= Other.m_Exposure;
		m_FStop				= Other.m_FStop;
		m_Gamma				= Other.m_Gamma;
		m_ToneMap			= Other.m_ToneMap;
		m_Bloom				= Other.m_Bloom;

		return *this;
	}

	// ToDo: Add description
	HOD void Update(const float& FovV, const float& Aperture)
	{
		float Scale = 0.0f;

		Scale = tanf(0.5f * (FovV / RAD_F));

		if (m_Resolution.m_AspectRatio > 1.0f)
		{
			m_Screen[0][0] = -Scale;
			m_Screen[0][1] = Scale;
			m_Screen[1][0] = -Scale / m_Resolution.m_AspectRatio;
			m_Screen[1][1] = Scale / m_Resolution.m_AspectRatio;
		}
		else
		{
			m_Screen[0][0] = -Scale / m_Resolution.m_AspectRatio;
			m_Screen[0][1] = Scale / m_Resolution.m_AspectRatio;
			m_Screen[1][0] = -Scale;
			m_Screen[1][1] = Scale;
		}

		m_InvScreen.x = (m_Screen[0][1] - m_Screen[0][0]) / m_Resolution.m_XY.x;
		m_InvScreen.y = (m_Screen[1][1] - m_Screen[1][0]) / m_Resolution.m_XY.y;

		m_Resolution.Update();
		m_ToneMap.Update();

		m_ToneMap.m_Factor = m_Exposure / (Aperture * Aperture) * m_Iso / 10.0f * powf(118.0f / 255.0f, m_Gamma);
	}
};

class FIDELITY_RENDER_DLL CImportance
{
public:
	float	m_Test;	
	bool	m_Enabled;			/*!< On/off */
	float	m_Radius;			/*!< Radius */
	float	m_Probability;		/*!< Probability */

	// ToDo: Add description
	HOD CImportance(void)
	{
		m_Enabled		= DEF_IMPORTANCE_ENABLED;
		m_Radius		= DEF_IMPORTANCE_RADIUS;
		m_Probability	= DEF_IMPORTANCE_PROBABILITY;
	}

	// ToDo: Add description
	HOD ~CImportance(void)
	{
	}

	// ToDo: Add description
	HOD CImportance& CImportance::operator=(const CImportance& Other)
	{
		m_Enabled		= Other.m_Enabled;
		m_Radius		= Other.m_Radius;
		m_Probability	= Other.m_Probability;

		return *this;
	}

	// ToDo: Add description
	HOD void Update(void)
	{
	}
};

class FIDELITY_RENDER_DLL CStereoRender
{
public:
	bool		m_Enabled;					/*!< On/off */
	float		m_EyeSeparation;			/*!< Eye separation */
	CColorXyz	m_LeftFilterColor;			/*!< Left filter color */
	CColorXyz	m_RightFilterColor;			/*!< Right filter color */

	// ToDo: Add description
	HOD CStereoRender(void)
	{
		m_Enabled				= false;
		m_EyeSeparation			= 0.25f;
		m_LeftFilterColor		= SPEC_WHITE;
		m_RightFilterColor		= SPEC_WHITE;
	}

	// ToDo: Add description
	HOD ~CStereoRender(void)
	{
	}

	// ToDo: Add description
	HOD CStereoRender& CStereoRender::operator=(const CStereoRender& Other)
	{
		m_Enabled				= Other.m_Enabled;
		m_EyeSeparation			= Other.m_EyeSeparation;
		m_LeftFilterColor		= Other.m_LeftFilterColor;
		m_RightFilterColor		= Other.m_RightFilterColor;

		return *this;
	}

	// ToDo: Add description
	HOD void Update(void)
	{
	}
};

#define FPS1 30.0f

class FIDELITY_RENDER_DLL CCameraAnimation
{
public:
	enum EType
	{
		TurnTable,
	};

	EType		m_Type;				/*!< Focus type e.g. turn table */
	bool		m_Enabled;			/*!< On/off */
	char		m_BaseName[255];	/*!< Base name */
	float		m_Duration;			/*!< Duration in seconds */
	int			m_CurrentFrame;		/*!< Current frame ID */
	int			m_NoFrames;			/*!< Total no. frames in animation */
	int			m_NoSamples;		/*!< No. samples per frame */
	float		m_Distance;			/*!< Distance from center */
	float		m_Latitude;			/*!< Up/down */
	float		m_InitialAngle;		/*!< Initial angle */
	float		m_MaxAngle;			/*!< Max angle */
	float		m_DeltaTheta;		/*!< Angle interval */

	// ToDo: Add description
	HOD CCameraAnimation(void)
	{
		m_Type			= TurnTable;
		m_Enabled		= false;

#ifndef __CUDACC__
		sprintf_s(m_BaseName, MAX_CHAR_SIZE, "animation_frame_");
#endif

		m_Duration		= 3.0f;
		m_CurrentFrame	= 0;
		m_NoSamples		= 250;
		m_Distance		= 5.0f;
		m_Latitude		= 0.0f;
		m_InitialAngle	= -90.0f;
		m_MaxAngle		= 360.0f;

		Update();
	}

	// ToDo: Add description
	HOD ~CCameraAnimation(void)
	{
	}

	// ToDo: Add description
	HOD CCameraAnimation& CCameraAnimation::operator=(const CCameraAnimation& Other)
	{
		m_Type			= Other.m_Type;
		m_Enabled		= Other.m_Enabled;

#ifndef __CUDACC__
		sprintf_s(m_BaseName, MAX_CHAR_SIZE, Other.m_BaseName);
#endif

		m_Duration		= Other.m_Duration;
		m_CurrentFrame	= Other.m_CurrentFrame;
		m_NoFrames		= Other.m_NoFrames;
		m_NoSamples		= Other.m_NoSamples;
		m_Distance		= Other.m_Distance;
		m_Latitude		= Other.m_Latitude;
		m_InitialAngle	= Other.m_InitialAngle;
		m_MaxAngle		= Other.m_MaxAngle;
		m_DeltaTheta	= Other.m_DeltaTheta;

		return *this;
	}

	// ToDo: Add description
	HOD void Sample(CCamera& Camera);

	// ToDo: Add description
	HOD void Start(void)
	{
		m_Enabled		= true;
		m_CurrentFrame	= 0;
	}

	// ToDo: Add description
	HOD void Stop(void)
	{
		m_Enabled		= false;
		m_CurrentFrame	= 0;
	}

	// ToDo: Add description
	HOD bool NextFrame(void)
	{
		m_CurrentFrame++;

		if (m_CurrentFrame >= m_NoFrames)
		{
			m_Enabled = false;
			return false;
		}

		return true;
	}

	// ToDo: Add description
	HOD void Update(void)
	{
		m_NoFrames		= m_Duration * FPS1;
		m_DeltaTheta	= (m_MaxAngle / RAD_F) / (float)m_NoFrames;
	}
};

class FIDELITY_RENDER_DLL CCamera 
{
public:
	enum EType
	{
		Perspective,
		Orthographic,
		Environment,
		Realistic,
		FishEye
	};

	EType				m_CameraType;
	CBoundingBox		m_SceneBoundingBox;
	float				m_Hither;
	float				m_Yon;
	bool				m_EnableClippingPlanes;
	Vec3f				m_From;
	Vec3f				m_Target;
	Vec3f				m_Up;
	float				m_FovV;
	float				m_AreaPixel;
	Vec3f 				m_N;
	Vec3f 				m_U;
	Vec3f 				m_V;
	CFilm				m_Film;
	CFocus				m_Focus;
	CAperture			m_Aperture;
	CImportance			m_Importance;
	CStereoRender		m_StereoRender;
	CCameraAnimation	m_Animation;
	bool				m_Dirty;

	// ToDo: Add description
	HOD CCamera(void)
	{
		m_CameraType			= DEF_CAMERA_TYPE;
		m_Hither				= DEF_CAMERA_HITHER;
		m_Yon					= DEF_CAMERA_YON;
		m_EnableClippingPlanes	= DEF_CAMERA_ENABLE_CLIPPING;
		m_From					= Vec3f(500.0f, 500.0f, 500.0f);
		m_Target				= Vec3f(0.0f, 0.0f, 0.0f);
		m_Up					= Vec3f(0.0f, 1.0f, 0.0f);
		m_FovV					= DEF_CAMERA_FIELD_OF_VIEW;
		m_N						= Vec3f(0.0f, 0.0f, 1.0f);
		m_U						= Vec3f(1.0f, 0.0f, 0.0f);
		m_V						= Vec3f(0.0f, 1.0f, 0.0f);
		m_Dirty					= true;
	}

	// ToDo: Add description
	HOD ~CCamera(void)
	{
	}

	// ToDo: Add description
	HOD CCamera& CCamera::operator=(const CCamera& Other)
	{
		m_CameraType			= Other.m_CameraType;
		m_SceneBoundingBox		= Other.m_SceneBoundingBox;
		m_Hither				= Other.m_Hither;
		m_Yon					= Other.m_Yon;
		m_EnableClippingPlanes	= Other.m_EnableClippingPlanes;
		m_From					= Other.m_From;
		m_Target				= Other.m_Target;
		m_Up					= Other.m_Up;
		m_FovV					= Other.m_FovV;
		m_AreaPixel				= Other.m_AreaPixel;
		m_N						= Other.m_N;
		m_U						= Other.m_U;
		m_V						= Other.m_V;
		m_Film					= Other.m_Film;
		m_Focus					= Other.m_Focus;
		m_Aperture				= Other.m_Aperture;
		m_Importance			= Other.m_Importance;
		m_StereoRender			= Other.m_StereoRender;
		m_Animation				= Other.m_Animation;
		m_Dirty					= Other.m_Dirty;

		return *this;
	}

	// ToDo: Add description
	HOD void Update(void)
	{
		m_N	= Normalize(m_Target - m_From);
		m_U	= Normalize(Cross(m_Up, m_N));
		m_V	= Normalize(Cross(m_N, m_U));
		
		m_Film.Update(m_FovV, m_Aperture.m_Size);

		m_AreaPixel = m_Film.m_Resolution.m_AspectRatio / (m_Focus.m_FocalDistance * m_Focus.m_FocalDistance);

		m_Aperture.Update(m_Film.m_FStop);

		m_StereoRender.m_EyeSeparation = 0.5f * (m_Focus.m_T / 30.0f);

		m_Film.Update(m_FovV, m_Aperture.m_Size);
		m_Animation.Update();
	}

	// Zoom operator
	HO void Zoom(float amount)
	{
		Vec3f reverseLoS = m_From - m_Target;

		if (amount > 0)
		{	
			// If positive, OperatorZoom out.
			reverseLoS.ScaleBy(1.1f);
		}
		else if (amount < 0)
		{	
			// Otherwise, OperatorZoom in
			if (reverseLoS.Length() > 0.0005f)
			{ 
				// Prevent the from point from hitting the at point.
				reverseLoS.ScaleBy(0.9f);
			}
		}

		m_From = reverseLoS + m_Target;
	}

	// Pan operator
	HO void Pan(float DownDegrees, float RightDegrees)
	{
		// Find the vector that defines the point to move
		Vec3f LoS = m_Target - m_From;

		// Find the orthogonal local scheme to use
		Vec3f right		= LoS.Cross(m_Up);
		Vec3f orthogUp	= LoS.Cross(right);

		// Normalize the vectors
		right.Normalize();
		orthogUp.Normalize();

		// Length from from to target
		const float Length = (m_Target - m_From).Length();

		// Obtain window width and height
		const unsigned int WindowWidth	= m_Film.m_Resolution.m_XY.x;

		// Compute translation along the screen
		const float U = Length * (RightDegrees / WindowWidth);
		const float V = Length * (DownDegrees / WindowWidth);

		// Compute new from and target
		m_From		= m_From + right * U - m_Up * V;
		m_Target	= m_Target + right * U - m_Up * V;
	}

	// Orbit operator
	HO void Orbit(float DownDegrees, float RightDegrees)
	{
		// Find the vector that defines the point to Move.
		Vec3f ReverseLoS	= m_From - m_Target;

		// Find the orthogonal local scheme to use.
		Vec3f right		= m_Up.Cross(ReverseLoS);
		Vec3f orthogUp	= ReverseLoS.Cross(right);
		Vec3f Up = Vec3f(0.0f, 1.0f, 0.0f);
		// Rotate both vectors in question around those axes we just found.
		ReverseLoS.RotateAxis(right, DownDegrees);
		ReverseLoS.RotateAxis(Up, RightDegrees);
		m_Up.RotateAxis(right, DownDegrees);
		m_Up.RotateAxis(Up, RightDegrees);

		// Store the vectors back into our variables. We
		// translate the reversed line of sight to return
		// it to a point. The up vector is just a vector,
		// so needs no translation.
		m_From = ReverseLoS + m_Target;
	}

	// Set the current view mode
	HO void SetViewMode(const EViewMode ViewMode)
	{
		if (ViewMode == ViewModeUser)
			return;

		// Set target and up vector
		m_Target	= m_SceneBoundingBox.GetCenter();
		m_Up		= Vec3f(0.0f, 1.0f, 0.0f);

		const float Distance = 1.5f;

		const float Length = Distance * m_SceneBoundingBox.GetMaxLength();

		m_From = m_Target;

		switch (ViewMode)
		{
			case ViewModeFront:							m_From.z -= Length;												break;
			case ViewModeBack:							m_From.z += Length;												break;
			case ViewModeLeft:							m_From.x += Length;												break;
			case ViewModeRight:							m_From.x -= -Length;											break;
			case ViewModeTop:							m_From.y += Length;		m_Up = Vec3f(0.0f, 0.0f, 1.0f);			break;
			case ViewModeBottom:						m_From.y -= -Length;	m_Up = Vec3f(0.0f, 0.0f, -1.0f);		break;
			case ViewModeIsometricFrontLeftTop:			m_From = Vec3f(Length, Length, -Length);						break;
			case ViewModeIsometricFrontRightTop:		m_From = m_Target + Vec3f(-Length, Length, -Length);			break;
			case ViewModeIsometricFrontLeftBottom:		m_From = m_Target + Vec3f(Length, -Length, -Length);			break;
			case ViewModeIsometricFrontRightBottom:		m_From = m_Target + Vec3f(-Length, -Length, -Length);			break;
			case ViewModeIsometricBackLeftTop:			m_From = m_Target + Vec3f(Length, Length, Length);				break;
			case ViewModeIsometricBackRightTop:			m_From = m_Target + Vec3f(-Length, Length, Length);				break;
			case ViewModeIsometricBackLeftBottom:		m_From = m_Target + Vec3f(Length, -Length, Length);				break;
			case ViewModeIsometricBackRightBottom:		m_From = m_Target + Vec3f(-Length, -Length, Length);			break;
		}

		Update();
	}

	// ToDo: Add description
	HOD void BiasDist(float& R) const
	{
		switch (m_Aperture.m_Bias)
		{
			case CAperture::BiasCenter:
				R = sqrtf(sqrtf(R) * R);
				break;
			
			case CAperture::BiasEdge:
				R = sqrtf((float)1.0f - R * R);
				break;

			default:
				R = R;
		}
	}

	// ToDo: Add description
	HOD Vec2f SampleTSD(const Vec2f& U) const
	{
		Vec2f u = U;

		float fn = (float)m_Aperture.m_NoBlades;
		
		int idx = int(u.x * fn);
		
		u.x = (u.x - ((float)idx) / fn) * fn;
		
//		BiasDist(u.x);
		
		const float b1 = u.x * u.y;
		const float b0 = u.x - b1;
		
		idx <<= 1;

		return Vec2f(m_Aperture.m_Data[idx] * b0 + m_Aperture.m_Data[idx + 2] * b1, m_Aperture.m_Data[idx + 1] * b0 + m_Aperture.m_Data[idx + 3] * b1);
	}

	// ToDo: Add description
	HOD Vec2f GetLensUV(const Vec2f& U)
	{
		// return ConcentricSampleDisk(U);

		switch (m_Aperture.m_NoBlades)
		{
			case 2:
				return ConcentricSampleDisk(U);

			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
				return SampleTSD(U);
		}

		return Vec2f(0.0f);
	}

	// Generates a (primary) camera ray
	HOD void GenerateRay(const Vec2f& Pixel, const Vec2f& ApertureRnd, Vec3f& RayO, Vec3f& RayD)
	{
		Vec2f ScreenPoint;

		ScreenPoint.x = m_Film.m_Screen[0][0] + (m_Film.m_InvScreen.x * Pixel.x);
		ScreenPoint.y = m_Film.m_Screen[1][0] + (m_Film.m_InvScreen.y * Pixel.y);

		RayO	= m_From;
		RayD	= Normalize(m_N + (-ScreenPoint.x * m_U) + (-ScreenPoint.y * m_V));

		if (m_Aperture.m_Size != 0.0f)
		{
			// Sample the lens
			Vec2f LensUV = m_Aperture.m_Size * GetLensUV(ApertureRnd);

			Vec3f LI = m_U * LensUV.x + m_V * LensUV.y;
			RayO += LI;
			RayD = Normalize((RayD * m_Focus.m_FocalDistance) - LI);
		}
	}

	// ToDo: Add description
	HOD bool Project(const Vec3f& w, float& u, float& v, float& pdf)
	{
		const Vec3f La	= ((1.0f / Dot(w, m_N)) * w);
		const Vec3f Laa	= La - m_N;
		const float U	= -Dot(Laa, m_U);
		const float V	= -Dot(Laa, m_V);

		if (Dot(w, m_N) < 0.0f || U < m_Film.m_Screen[0][0] || U > m_Film.m_Screen[0][1] || V < m_Film.m_Screen[1][0] || V > m_Film.m_Screen[1][1])
			return false;

		const float Ua	= U / fabs(m_Film.m_Screen[0][1]);
		const float Va	= V / fabs(m_Film.m_Screen[1][1]);

		const float HalfW 	= 0.5f * m_Film.m_Resolution.m_XY.x;
		const float HalfH 	= 0.5f * m_Film.m_Resolution.m_XY.y;

		u	= HalfW + Ua * HalfW;
		v	= HalfH + Va * HalfH;

		// pdf = 1/A_pix * r^2 / cos(forward, dir), where r^2 is also 1/cos(vto, dir)^2
		float cos_wo = AbsDot(w, m_N); //camZ * wo.dir;

		pdf = 8.f * PI_F / (m_AreaPixel *  cos_wo * cos_wo * cos_wo );

		return true;
	}
};