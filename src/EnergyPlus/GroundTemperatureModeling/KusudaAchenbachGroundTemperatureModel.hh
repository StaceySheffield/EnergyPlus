#ifndef KusudaAchenbachGroundTemperatureModel_hh_INCLUDED
#define KusudaAchenbachGroundTemperatureModel_hh_INCLUDED

// C++ Headers
#include <memory>

// EnergyPlus Headers
#include <DataGlobals.hh>
#include <EnergyPlus.hh>
#include <GroundTemperatureModeling/BaseGroundTemperatureModel.hh>

namespace EnergyPlus{

	// Derived class for Kusuda-Achenbach model
	class KusudaGroundTempsModel : public BaseGroundTempsModel
	{
		public:
			// Public Members
			Real64 depth;
			Real64 groundThermalDiffisivity;
			Real64 simTimeInSeconds;
			Real64 aveGroundTemp;
			Real64 aveGroundTempAmplitude;
			Real64 phaseShiftInSecs;

		static std::shared_ptr< KusudaGroundTempsModel > 
		KusudaGTMFactory( 
			int objectType, 
			std::string objectName
		);

		Real64
		getGroundTemp();

		Real64
		getGroundTempAtTimeInSeconds(
			Real64 const depth,
			Real64 const timeInSecondsOfSim
		);

		Real64
		getGroundTempAtTimeInMonths(
			Real64 const depth,
			int const monthOfSim
		);

	};

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

}

#endif
