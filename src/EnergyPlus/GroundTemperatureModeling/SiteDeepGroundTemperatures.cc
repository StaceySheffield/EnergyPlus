// C++ Headers
#include <memory>

// ObjexxFCL Headers
#include <ObjexxFCL/gio.hh>

// EnergyPlus Headers
#include <DataEnvironment.hh>
#include <DataGlobals.hh>
#include <DataIPShortCuts.hh>
#include <GroundTemperatureModeling/GroundTemperatureModelManager.hh>
#include <GroundTemperatureModeling/SiteDeepGroundTemperatures.hh>
#include <InputProcessor.hh>
#include <WeatherManager.hh>

namespace EnergyPlus {

	static gio::Fmt fmtA( "(A)" );
	static gio::Fmt fmtAN( "(A,$)" );

	//******************************************************************************

	// Site:GroundTemperature:Deep factory
	std::shared_ptr< SiteDeepGroundTemps > 
	SiteDeepGroundTemps::DeepGTMFactory( 
		int objectType, 
		std::string objectName
	)
	{
		// SUBROUTINE INFORMATION:
		//       AUTHOR         Matt Mitchell
		//       DATE WRITTEN   Summer 2015
		//       MODIFIED       na
		//       RE-ENGINEERED  na

		// PURPOSE OF THIS SUBROUTINE:
		// Reads input and creates instance of Site:GroundTemperature:Deep object

		// USE STATEMENTS:
		using DataEnvironment::GroundTemp_DeepObjInput;
		using DataGlobals::OutputFileInits;
		using namespace DataIPShortCuts;
		using namespace GroundTemperatureManager;
		using namespace ObjexxFCL::gio;

		// Locals
		// SUBROUTINE LOCAL VARIABLE DECLARATIONS:
		bool found = false;
		int NumNums;
		int NumAlphas;
		int IOStat;

		// New shared pointer for this model object
		std::shared_ptr< SiteDeepGroundTemps > thisModel( new SiteDeepGroundTemps() );

		std::string const cCurrentModuleObject = CurrentModuleObjects( objectType_SiteDeepGroundTemp );
		int numCurrObjects = InputProcessor::GetNumObjectsFound( cCurrentModuleObject );

		thisModel->objectType = objectType;
		thisModel->objectName = objectName;

		if ( numCurrObjects == 1 ) {

			//Get the object names for each construction from the input processor
			InputProcessor::GetObjectItem( cCurrentModuleObject, 1, cAlphaArgs, NumAlphas, rNumericArgs, NumNums, IOStat );

			if ( NumNums < 12 ) {
				ShowSevereError( cCurrentModuleObject + ": Less than 12 values entered." );
				thisModel->errorsFound = true;
			}

			// overwrite values read from weather file for the 0.5m set ground temperatures
			for ( int i = 1; i <= 12; ++i ) {
				thisModel->deepGroundTemps( i ) = rNumericArgs( i );
			}

			GroundTemp_DeepObjInput = true;

		} else if ( numCurrObjects > 1 ) {
			ShowSevereError( cCurrentModuleObject + ": Too many objects entered. Only one allowed." );
			thisModel->errorsFound = true;

		} else {
			thisModel->deepGroundTemps = 16.0;
		}

		// Write Final Ground Temp Information to the initialization output file
		gio::write( OutputFileInits, fmtA ) << "! <Site:GroundTemperature:Deep>, Months From Jan to Dec {C}";
		gio::write( OutputFileInits, fmtAN ) << " Site:GroundTemperature:Deep";
		for ( int i = 1; i <= 12; ++i ) gio::write( OutputFileInits, "(', ',F6.2,$)" ) << thisModel->deepGroundTemps( i ); gio::write( OutputFileInits );

		found = true;

		if ( found && !thisModel->errorsFound ) {
			groundTempModels.push_back( thisModel );
			return thisModel;
		} else {
			ShowContinueError( "Site:GroundTemperature:Deep--Errors getting input for ground temperature model");
			return nullptr;
		}
	}	

	//******************************************************************************

	Real64
	SiteDeepGroundTemps::getGroundTemp()
	{
		// SUBROUTINE INFORMATION:
		//       AUTHOR         Matt Mitchell
		//       DATE WRITTEN   Summer 2015
		//       MODIFIED       na
		//       RE-ENGINEERED  na

		// PURPOSE OF THIS SUBROUTINE:
		// Returns the ground temperature for Site:GroundTemperature:Deep

		return deepGroundTemps( timeOfSimInMonths );
	}

	//******************************************************************************

	Real64
	SiteDeepGroundTemps::getGroundTempAtTimeInSeconds(
		Real64 const EP_UNUSED( _depth ),
		Real64 const _seconds
	)
	{
		// SUBROUTINE INFORMATION:
		//       AUTHOR         Matt Mitchell
		//       DATE WRITTEN   Summer 2015
		//       MODIFIED       na
		//       RE-ENGINEERED  na

		// PURPOSE OF THIS SUBROUTINE:
		// Returns the ground temperature when input time is in seconds

		// USE STATEMENTS:
		using DataGlobals::SecsInDay;
		using WeatherManager::NumDaysInYear;

		// SUBROUTINE LOCAL VARIABLE DECLARATIONS:
		Real64 secPerMonth = NumDaysInYear * SecsInDay / 12;

		// Convert secs to months
		int month = ceil( _seconds / secPerMonth );

		if ( month >= 1 && month <= 12 ) {
			timeOfSimInMonths = month;
		} else {
			timeOfSimInMonths = remainder( month, 12 );
		}

		// Get and return ground temp
		return getGroundTemp();
	}

	//******************************************************************************

	Real64
	SiteDeepGroundTemps::getGroundTempAtTimeInMonths(
		Real64 const EP_UNUSED( _depth ),
		int const _month
	)
	{
		// SUBROUTINE INFORMATION:
		//       AUTHOR         Matt Mitchell
		//       DATE WRITTEN   Summer 2015
		//       MODIFIED       na
		//       RE-ENGINEERED  na

		// PURPOSE OF THIS SUBROUTINE:
		// Returns the ground temperature when input time is in months

		// Set month
		if ( _month >= 1 && _month <= 12 ) {
			timeOfSimInMonths = _month;
		} else {
			timeOfSimInMonths = remainder( _month, 12 );
		}

		// Get and return ground temp
		return getGroundTemp();
	}

	//******************************************************************************

	//     NOTICE

	//     Copyright (c) 1996-2015 The Board of Trustees of the University of Illinois
	//     and The Regents of the University of California through Ernest Orlando Lawrence
	//     Berkeley National Laboratory.  All rights reserved.

	//     Portions of the EnergyPlus software package have been developed and copyrighted
	//     by other individuals, companies and institutions.  These portions have been
	//     incorporated into the EnergyPlus software package under license.   For a complete
	//     list of contributors, see "Notice" located in main.cc.

	//     NOTICE: The U.S. Government is granted for itself and others acting on its
	//     behalf a paid-up, nonexclusive, irrevocable, worldwide license in this data to
	//     reproduce, prepare derivative works, and perform publicly and display publicly.
	//     Beginning five (5) years after permission to assert copyright is granted,
	//     subject to two possible five year renewals, the U.S. Government is granted for
	//     itself and others acting on its behalf a paid-up, non-exclusive, irrevocable
	//     worldwide license in this data to reproduce, prepare derivative works,
	//     distribute copies to the public, perform publicly and display publicly, and to
	//     permit others to do so.

	//     TRADEMARKS: EnergyPlus is a trademark of the US Department of Energy.

}	// EnergyPlus
