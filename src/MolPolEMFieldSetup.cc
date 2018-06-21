#include "MolPolEMFieldSetup.hh"
#include "MolPolEMFieldMessenger.hh"
#include "MolPolEMField.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4EquationOfMotion.hh"
#include "G4EqMagElectricField.hh"
#include "MolPolQuad.hh"
#include "MolPolDipole.hh"
#include "MolPolSolenoid.hh"
#include "MolPolEMField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"

#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"
#include "G4ios.hh"

#include <iostream>
#include <fstream>

using namespace std;


MolPolEMFieldSetup::MolPolEMFieldSetup()
  : fFieldManager(0),
    fChordFinder(0),
    fStepper(0),
    fIntgrDriver(0),
    fFieldMessenger(0),
    fStepperType(0),
    fMinStep(0)
{

  fFieldMessenger = new MolPolEMFieldMessenger(this);

  fMagSourceMode = 0;
  fQ1A = 0;
  fQ2A = 0;
  fQ3A = 0;
  fQ4A = 0;
  fQ5A = 0;
  fQ6A = 0;

  fQ1T = 0;
  fQ2T = 0;
  fQ3T = 0;
  fQ4T = 0;
  fQ5T = 0;
  fQ6T = 0;

  InitialseAll();

}

void MolPolEMFieldSetup::InitialseAll()
{

  //FIXME: THIS CONSTRUCTS A GLOBAL FIELD THAT IS CURRENTLY ZERO... ADD SECOND
  //CONSTRUCTOR AND MOVE THE fEMfield INTO THE CASES... I GUESS.
  //fEMfield = new MolPolEMField();
  //fEquation = new G4EqMagElectricField(fEMfield);
  //fMinStep  = 0.01*mm ; // minimal step of 1 miron, default is 0.01 mm: Doesn't seem to make much difference here
  //fStepperType = 4 ;    // ClassicalRK4 -- the default stepper

  //fFieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();

  //fChordFinder = 0;   //will be set in UpdateField()
  //UpdateField();

  G4RotationMatrix* NOROT = new G4RotationMatrix;

  ORIGINQ1 =  75.19 * cm;
  ORIGINQ2 = 140.46 * cm;
  ORIGINQ3 = 209.08 * cm;
  ORIGINQ4 = 274.59 * cm;
  ORIGIND  = 423.4  * cm;
  ORIGINQ6 = 6.9    * cm;

  BORERADIUS = 5.08 * cm;

  KAPPA1 = 0.;
  KAPPA2 = 0.;
  KAPPA3 = 0.;
  KAPPA4 = 0.;
  DIPOLE = 0.;
  SOLENOID = 0.;

  if( fMagSourceMode == 0 ){

    fEMfield = new MolPolEMField();
    fEquation = new G4EqMagElectricField(fEMfield);
    fMinStep  = 0.01*mm ; // minimal step of 1 miron, default is 0.01 mm: Doesn't seem to make much difference here
    fStepperType = 4 ;    // ClassicalRK4 -- the default stepper

    fFieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();

    fChordFinder = 0;   //will be set in UpdateField()
    UpdateField();

    G4cout << "(InitializeAll) Case 0 Setup Detetcted..." << G4endl;

    KAPPA1 = CalA2T(fQ1A, 1) / BORERADIUS;
    KAPPA2 = CalA2T(fQ2A, 2) / BORERADIUS;
    KAPPA3 = CalA2T(fQ3A, 3) / BORERADIUS;
    KAPPA4 = CalA2T(fQ4A, 4) / BORERADIUS;
    DIPOLE = CalA2T(fQ5A, 5);
    SOLENOID = fQ6T * tesla;

    fMagFieldFZB1 = new MolPolQuad(KAPPA1, G4ThreeVector(0.0, 0.0, ORIGINQ1), NOROT, BORERADIUS);
    fEquationFZB1 = new G4Mag_UsualEqRhs(fMagFieldFZB1);
    fStepperFZB1  = new G4ClassicalRK4(fEquationFZB1);
    fLocalFieldManagerFZB1 = new G4FieldManager();
    fChordFinderFZB1 = 0;
    UpdateFieldFZB1();

    fMagFieldFZB2 = new MolPolQuad(KAPPA2, G4ThreeVector(0.0, 0.0, ORIGINQ2), NOROT, BORERADIUS);
    fEquationFZB2 = new G4Mag_UsualEqRhs(fMagFieldFZB2);
    fStepperFZB2  = new G4ClassicalRK4(fEquationFZB2);
    fLocalFieldManagerFZB2 = new G4FieldManager();
    fChordFinderFZB2 = 0;
    UpdateFieldFZB2();

    fMagFieldFZB3 = new MolPolQuad(KAPPA3, G4ThreeVector(0.0, 0.0, ORIGINQ3), NOROT, BORERADIUS);
    fEquationFZB3 = new G4Mag_UsualEqRhs(fMagFieldFZB3);
    fStepperFZB3  = new G4ClassicalRK4(fEquationFZB3);
    fLocalFieldManagerFZB3 = new G4FieldManager();
    fChordFinderFZB3 = 0;
    UpdateFieldFZB3();

    fMagFieldFZB4 = new MolPolQuad(KAPPA4, G4ThreeVector(0.0, 0.0, ORIGINQ4), NOROT, BORERADIUS);
    fEquationFZB4 = new G4Mag_UsualEqRhs(fMagFieldFZB4);
    fStepperFZB4  = new G4ClassicalRK4(fEquationFZB4);
    fLocalFieldManagerFZB4 = new G4FieldManager();
    fChordFinderFZB4 = 0;
    UpdateFieldFZB4();

    fMagFieldFZB5 = new MolPolDipole(DIPOLE, G4ThreeVector(0.0, 0.0, ORIGIND), NOROT);
    fEquationFZB5 = new G4Mag_UsualEqRhs(fMagFieldFZB5);
    fStepperFZB5  = new G4ClassicalRK4(fEquationFZB5);
    fLocalFieldManagerFZB5 = new G4FieldManager();
    fChordFinderFZB5 = 0;
    UpdateFieldFZB5();

    fMagFieldFZB6 = new MolPolSolenoid(SOLENOID, 0, G4ThreeVector(0.0, 0.0, ORIGINQ6));
    fEquationFZB6 = new G4Mag_UsualEqRhs(fMagFieldFZB6);
    fStepperFZB6  = new G4ClassicalRK4(fEquationFZB6);
    fLocalFieldManagerFZB6 = new G4FieldManager();
    fChordFinderFZB6 = 0;
    UpdateFieldFZB6();

  } else if(fMagSourceMode == 1) {

    fEMfield = new MolPolEMField();
    fEquation = new G4EqMagElectricField(fEMfield);
    fMinStep  = 0.01*mm ; // minimal step of 1 miron, default is 0.01 mm: Doesn't seem to make much difference here
    fStepperType = 4 ;    // ClassicalRK4 -- the default stepper

    fFieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();

    fChordFinder = 0;   //will be set in UpdateField()
    UpdateField();

    G4cout << "(InitializeAll) Case 1 Setup Detetcted..." << G4endl;

    KAPPA1 = fQ1T * tesla / BORERADIUS;
    KAPPA2 = fQ2T * tesla / BORERADIUS;
    KAPPA3 = fQ3T * tesla / BORERADIUS;
    KAPPA4 = fQ4T * tesla / BORERADIUS;
    DIPOLE = fQ5T * tesla;
    SOLENOID = fQ6T * tesla;

    fMagFieldFZB1 = new MolPolQuad(KAPPA1, G4ThreeVector(0.0, 0.0, ORIGINQ1), NOROT, BORERADIUS);
    fEquationFZB1 = new G4Mag_UsualEqRhs(fMagFieldFZB1);
    fStepperFZB1  = new G4ClassicalRK4(fEquationFZB1);
    fLocalFieldManagerFZB1 = new G4FieldManager();
    fChordFinderFZB1 = 0;
    UpdateFieldFZB1();

    fMagFieldFZB2 = new MolPolQuad(KAPPA2, G4ThreeVector(0.0, 0.0, ORIGINQ2), NOROT, BORERADIUS);
    fEquationFZB2 = new G4Mag_UsualEqRhs(fMagFieldFZB2);
    fStepperFZB2  = new G4ClassicalRK4(fEquationFZB2);
    fLocalFieldManagerFZB2 = new G4FieldManager();
    fChordFinderFZB2 = 0;
    UpdateFieldFZB2();

    fMagFieldFZB3 = new MolPolQuad(KAPPA3, G4ThreeVector(0.0, 0.0, ORIGINQ3), NOROT, BORERADIUS);
    fEquationFZB3 = new G4Mag_UsualEqRhs(fMagFieldFZB3);
    fStepperFZB3  = new G4ClassicalRK4(fEquationFZB3);
    fLocalFieldManagerFZB3 = new G4FieldManager();
    fChordFinderFZB3 = 0;
    UpdateFieldFZB3();

    fMagFieldFZB4 = new MolPolQuad(KAPPA4, G4ThreeVector(0.0, 0.0, ORIGINQ4), NOROT, BORERADIUS);
    fEquationFZB4 = new G4Mag_UsualEqRhs(fMagFieldFZB4);
    fStepperFZB4  = new G4ClassicalRK4(fEquationFZB4);
    fLocalFieldManagerFZB4 = new G4FieldManager();
    fChordFinderFZB4 = 0;
    UpdateFieldFZB4();

    fMagFieldFZB5 = new MolPolDipole(DIPOLE, G4ThreeVector(0.0, 0.0, ORIGIND), NOROT);
    fEquationFZB5 = new G4Mag_UsualEqRhs(fMagFieldFZB5);
    fStepperFZB5  = new G4ClassicalRK4(fEquationFZB5);
    fLocalFieldManagerFZB5 = new G4FieldManager();
    fChordFinderFZB5 = 0;
    UpdateFieldFZB5();

    fMagFieldFZB6 = new MolPolSolenoid(SOLENOID, 0, G4ThreeVector(0.0, 0.0, ORIGINQ6));
    fEquationFZB6 = new G4Mag_UsualEqRhs(fMagFieldFZB6);
    fStepperFZB6  = new G4ClassicalRK4(fEquationFZB6);
    fLocalFieldManagerFZB6 = new G4FieldManager();
    fChordFinderFZB6 = 0;
    UpdateFieldFZB6();

  } else if(fMagSourceMode == 2) {

    G4cout << "#############################################################" << G4endl;
    G4cout << "#                    _                                      #" << G4endl;
    G4cout << "#                  -=\\`\\                                    #" << G4endl;
    G4cout << "#              |\\ ____\\_\\___                                #" << G4endl;
    G4cout << "#            -=\\ MollerAir****`)                            #" << G4endl;
    G4cout << "#               `~~~~~/ /~~~~~~`                            #" << G4endl;
    G4cout << "#                 -==/ /                                    #" << G4endl;
    G4cout << "#                   '-'                                     #" << G4endl;
    G4cout << "#                  _  _                                     #" << G4endl;
    G4cout << "#                 ( `   )_                                  #" << G4endl;
    G4cout << "#                (    )    `)                               #" << G4endl;
    G4cout << "#              (_   (_ .  _) _)                             #" << G4endl;
    G4cout << "#                                             _             #" << G4endl;
    G4cout << "#                                            (  )           #" << G4endl;
    G4cout << "#             _ .                         ( `  ) . )        #" << G4endl;
    G4cout << "#           (  _ )_                      (_, _(  ,_)_)      #" << G4endl;
    G4cout << "#         (_  _(_ ,)                                        #" << G4endl;
    G4cout << "#############################################################" << G4endl;

    TOSCAQ1 = fToscaQ1;
    TOSCAQ2 = fToscaQ2;
    TOSCADP = fToscaDp;

    std::ifstream finq1(TOSCAQ1);
    std::ifstream finq2(TOSCAQ2);
    std::ifstream findp(TOSCADP);

    if(!finq1){
      G4cout << "File " << TOSCAQ1 << " does not exist..." << G4endl;
      G4Exception("MolPolEMFieldSetup::InitializeAll()","Invalid File Location", FatalException,"File for TOSCAQ1 does not exist...");
    }
    if(!finq2){
      G4cout << "File " << TOSCAQ2 << " does not exist..." << G4endl;
      G4Exception("MolPolEMFieldSetup::InitializeAll()","Invalid File Location", FatalException,"File for TOSCAQ2 does not exist...");
    }
    if(!findp){
      G4cout << "File " << TOSCADP << " does not exist..." << G4endl;
      G4Exception("MolPolEMFieldSetup::InitializeAll()","Invalid File Location", FatalException,"File for TOSCADP does not exist...");
    }

    //FIXME: SHOULD PROBABLY CLEAR THESE JUST IN CASE BEFORE USAGE
    filelist.push_back(fToscaQ1);
    filelist.push_back(fToscaQ2);
    filelist.push_back(fToscaQ1);
    filelist.push_back(fToscaQ1);
    filelist.push_back(fToscaDp);
    filescales.push_back(fToscaQ1scale);
    filescales.push_back(fToscaQ2scale);
    filescales.push_back(fToscaQ3scale);
    filescales.push_back(fToscaQ4scale);
    filescales.push_back(fToscaDpscale);

    fEMfield = new MolPolEMField( filelist , filescales );
    fEquation = new G4EqMagElectricField(fEMfield);
    fMinStep  = 0.01*mm ; // minimal step of 1 miron, default is 0.01 mm: Doesn't seem to make much difference here
    fStepperType = 4 ;    // ClassicalRK4 -- the default stepper

    fFieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();

    fChordFinder = 0;   //will be set in UpdateField()
    UpdateField();

  }

}

/////////////////////////////////////////////////////////////////////////////////
//

MolPolEMFieldSetup::~MolPolEMFieldSetup()
{
  if(fChordFinder)    delete fChordFinder;
  if(fStepper)        delete fStepper;
  if(fEquation)       delete fEquation;
  if(fEMfield)        delete fEMfield;
  if(fFieldMessenger) delete fFieldMessenger;
}
/////////////////////////////////////////////////////////////////////////////////
//

void MolPolEMFieldSetup::UpdateConfiguration(){

  //FIXME: UPDATE CONFIGURATION SHOULD NOW PROBABLY DELETE OLD FIELDS AND REMAKE.

  G4RotationMatrix* NOROT = new G4RotationMatrix;

  if( fMagSourceMode == 1 ){
    KAPPA1 = CalA2T(fQ1A, 1) / BORERADIUS;
    KAPPA2 = CalA2T(fQ2A, 2) / BORERADIUS;
    KAPPA3 = CalA2T(fQ3A, 3) / BORERADIUS;
    KAPPA4 = CalA2T(fQ4A, 4) / BORERADIUS;
    DIPOLE = CalA2T(fQ5A, 5);

    SOLENOID = fQ6T * tesla;

    G4cout << "(Update Case: 0) Received values from macro: " << G4endl;
    G4cout << "\tfQ1T: " << fQ1T << G4endl;
    G4cout << "\tfQ2T: " << fQ2T << G4endl;
    G4cout << "\tfQ3T: " << fQ3T << G4endl;
    G4cout << "\tfQ4T: " << fQ4T << G4endl;
    G4cout << "\tfQ5T: " << fQ5T << G4endl;
    G4cout << "\tfQ6T: " << fQ6T << G4endl;

    G4cout << __PRETTY_FUNCTION__ <<"\t at line: "<<__LINE__<<G4endl;
    G4cout << "\tfMagSourceMode: "<<fMagSourceMode<<G4endl
  	 << "\tKAPPA1: "<<KAPPA1/(tesla / m)<< " tesla/m"<<G4endl
  	 << "\tKAPPA1: "<<KAPPA2/(tesla / m)<< " tesla/m"<<G4endl
  	 << "\tKAPPA1: "<<KAPPA3/(tesla / m)<< " tesla/m"<<G4endl
  	 << "\tKAPPA1: "<<KAPPA4/(tesla / m)<< " tesla/m"<<G4endl
  	 << "\tDIPOLE: "<<DIPOLE/tesla<< " tesla"<<G4endl
     << "\tSOLEND: "<<SOLENOID/tesla<< " tesla"<<G4endl;

    ((MolPolQuad*)fMagFieldFZB1)->UpdateQuad(KAPPA1, G4ThreeVector(0.0, 0.0, ORIGINQ1), NOROT, BORERADIUS);
    ((MolPolQuad*)fMagFieldFZB2)->UpdateQuad(KAPPA2, G4ThreeVector(0.0, 0.0, ORIGINQ2), NOROT, BORERADIUS);
    ((MolPolQuad*)fMagFieldFZB3)->UpdateQuad(KAPPA3, G4ThreeVector(0.0, 0.0, ORIGINQ3), NOROT, BORERADIUS);
    ((MolPolQuad*)fMagFieldFZB4)->UpdateQuad(KAPPA4, G4ThreeVector(0.0, 0.0, ORIGINQ4), NOROT, BORERADIUS);
    ((MolPolDipole*)fMagFieldFZB5)->UpdateDipole(DIPOLE, G4ThreeVector(0.0, 0.0, ORIGIND), NOROT);
    ((MolPolSolenoid*)fMagFieldFZB6)->UpdateSolenoid(SOLENOID, 0, G4ThreeVector(0.0, 0.0, ORIGINQ6));

  } else if(fMagSourceMode == 1) {

    KAPPA1 = fQ1T * tesla / BORERADIUS;
    KAPPA2 = fQ2T * tesla / BORERADIUS;
    KAPPA3 = fQ3T * tesla / BORERADIUS;
    KAPPA4 = fQ4T * tesla / BORERADIUS;
    DIPOLE = fQ5T * tesla;

    SOLENOID = fQ6T * tesla;

    G4cout << "(Update Case: 1) Received values from macro: " << G4endl
           << "\tfQ1T: " << fQ1T << G4endl
           << "\tfQ2T: " << fQ2T << G4endl
           << "\tfQ3T: " << fQ3T << G4endl
           << "\tfQ4T: " << fQ4T << G4endl
           << "\tfQ5T: " << fQ5T << G4endl
           << "\tfQ6T: " << fQ6T << G4endl;

    G4cout << __PRETTY_FUNCTION__ << "\t at line: " << __LINE__ << G4endl
           << "\tfMagSourceMode: " << fMagSourceMode << G4endl
  	       << "\tKAPPA1: " <<KAPPA1/(tesla / m) << " tesla/m" << G4endl
  	       << "\tKAPPA1: " <<KAPPA2/(tesla / m) << " tesla/m" << G4endl
  	       << "\tKAPPA1: " <<KAPPA3/(tesla / m) << " tesla/m" << G4endl
  	       << "\tKAPPA1: " <<KAPPA4/(tesla / m) << " tesla/m" << G4endl
  	       << "\tDIPOLE: " <<DIPOLE/tesla << " tesla" << G4endl
           << "\tSOLEND: " <<SOLENOID/tesla << " tesla" << G4endl;

    ((MolPolQuad*)fMagFieldFZB1)->UpdateQuad(KAPPA1, G4ThreeVector(0.0, 0.0, ORIGINQ1), NOROT, BORERADIUS);
    ((MolPolQuad*)fMagFieldFZB2)->UpdateQuad(KAPPA2, G4ThreeVector(0.0, 0.0, ORIGINQ2), NOROT, BORERADIUS);
    ((MolPolQuad*)fMagFieldFZB3)->UpdateQuad(KAPPA3, G4ThreeVector(0.0, 0.0, ORIGINQ3), NOROT, BORERADIUS);
    ((MolPolQuad*)fMagFieldFZB4)->UpdateQuad(KAPPA4, G4ThreeVector(0.0, 0.0, ORIGINQ4), NOROT, BORERADIUS);
    ((MolPolDipole*)fMagFieldFZB5)->UpdateDipole(DIPOLE, G4ThreeVector(0.0, 0.0, ORIGIND), NOROT);
    ((MolPolSolenoid*)fMagFieldFZB6)->UpdateSolenoid(SOLENOID, 0, G4ThreeVector(0.0, 0.0, ORIGINQ6));

  }else if (fMagSourceMode == 2){

    G4cout << "#############################################################" << G4endl;
    G4cout << "#                    _                                      #" << G4endl;
    G4cout << "#                  -=\\`\\                                    #" << G4endl;
    G4cout << "#              |\\ ____\\_\\___                                #" << G4endl;
    G4cout << "#            -=\\ MollerAir****`)                            #" << G4endl;
    G4cout << "#               `~~~~~/ /~~~~~~`                            #" << G4endl;
    G4cout << "#                 -==/ /                                    #" << G4endl;
    G4cout << "#                   '-'                                     #" << G4endl;
    G4cout << "#                  _  _                                     #" << G4endl;
    G4cout << "#                 ( `   )_                                  #" << G4endl;
    G4cout << "#                (    )    `)                               #" << G4endl;
    G4cout << "#              (_   (_ .  _) _)                             #" << G4endl;
    G4cout << "#                                             _             #" << G4endl;
    G4cout << "#                                            (  )           #" << G4endl;
    G4cout << "#             _ .                         ( `  ) . )        #" << G4endl;
    G4cout << "#           (  _ )_                      (_, _(  ,_)_)      #" << G4endl;
    G4cout << "#         (_  _(_ ,)                                        #" << G4endl;
    G4cout << "#############################################################" << G4endl;

    TOSCAQ1 = fToscaQ1;
    TOSCAQ2 = fToscaQ2;
    TOSCADP = fToscaDp;

    std::ifstream finq1(TOSCAQ1);
    std::ifstream finq2(TOSCAQ2);
    std::ifstream findp(TOSCADP);

    G4cout << "Checking to see if files exist..." << G4endl;

    if(!finq1){
      G4cout << "File " << TOSCAQ1 << " does not exist..." << G4endl;
      G4Exception("MolPolEMFieldSetup::InitializeAll()","Invalid File Location", FatalException,"File for TOSCAQ1 does not exist...");
    } else {
      G4cout << "File " << TOSCAQ1 << " exists..." << G4endl;
    }
    if(!finq2){
      G4cout << "File " << TOSCAQ2 << " does not exist..." << G4endl;
      G4Exception("MolPolEMFieldSetup::InitializeAll()","Invalid File Location", FatalException,"File for TOSCAQ2 does not exist...");
    } else {
      G4cout << "File " << TOSCAQ2 << " exists..." << G4endl;
    }
    if(!findp){
      G4cout << "File " << TOSCADP << " does not exist..." << G4endl;
      G4Exception("MolPolEMFieldSetup::InitializeAll()","Invalid File Location", FatalException,"File for TOSCADP does not exist...");
    } else {
      G4cout << "File " << TOSCADP << " exists..." << G4endl;
    }

    std::vector<G4String> filelist;
    filelist.push_back(fToscaQ1);
    filelist.push_back(fToscaQ2);
    filelist.push_back(fToscaQ1);
    filelist.push_back(fToscaQ1);
    filelist.push_back(fToscaDp);
    std::vector<G4double> filescales;
    filescales.push_back(fToscaQ1scale);
    filescales.push_back(fToscaQ2scale);
    filescales.push_back(fToscaQ3scale);
    filescales.push_back(fToscaQ4scale);
    filescales.push_back(fToscaDpscale);

    fEMfield = new MolPolEMField( filelist , filescales );
    fEquation = new G4EqMagElectricField(fEMfield);
    fMinStep  = 0.01*mm ; // minimal step of 1 miron, default is 0.01 mm: Doesn't seem to make much difference here
    fStepperType = 4 ;    // ClassicalRK4 -- the default stepper

    fFieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();

    fChordFinder = 0;   //will be set in UpdateField()
    UpdateField();

    } else {
      G4cout << "Warning, no proper choice set for 'MagSourceMode'. You should terminate the simulation." << G4endl;
    }


}

/////////////////////////////////////////////////////////////////////////////
//
// Register this field to 'global' Field Manager and
// Create Stepper and Chord Finder with predefined type, minstep (resp.)
//

void MolPolEMFieldSetup::UpdateField()
{
  fStepper = new G4ClassicalRK4( fEquation, 8 );

  fFieldManager->SetDetectorField(fEMfield);

  if(fChordFinder) delete fChordFinder;
  fIntgrDriver = new G4MagInt_Driver(fMinStep,fStepper,fStepper->GetNumberOfVariables());
  fChordFinder = new G4ChordFinder(fIntgrDriver);
  fFieldManager->SetChordFinder( fChordFinder );
}


/////////////////////////////////////////////////////////////////////////////
void MolPolEMFieldSetup::UpdateFieldFZB1()
{

  fLocalFieldManagerFZB1->SetDetectorField(fMagFieldFZB1);

  if(fChordFinderFZB1) delete fChordFinderFZB1;
  fIntgrDriverFZB1 = new G4MagInt_Driver(fMinStep,fStepperFZB1,fStepperFZB1->GetNumberOfVariables());
  fChordFinderFZB1 = new G4ChordFinder((G4MagneticField*) fMagFieldFZB1, fMinStep, fStepperFZB1);
  fLocalFieldManagerFZB1->SetChordFinder( fChordFinderFZB1 );

}

/////////////////////////////////////////////////////////////////////////////
void MolPolEMFieldSetup::UpdateFieldFZB2()
{

  fLocalFieldManagerFZB2->SetDetectorField(fMagFieldFZB2);

  if(fChordFinderFZB2) delete fChordFinderFZB2;
  fIntgrDriverFZB2 = new G4MagInt_Driver(fMinStep,fStepperFZB2,fStepperFZB2->GetNumberOfVariables());
  fChordFinderFZB2 = new G4ChordFinder((G4MagneticField*) fMagFieldFZB2, fMinStep, fStepperFZB2);
  fLocalFieldManagerFZB2->SetChordFinder( fChordFinderFZB2 );

}

/////////////////////////////////////////////////////////////////////////////
void MolPolEMFieldSetup::UpdateFieldFZB3()
{

  fLocalFieldManagerFZB3->SetDetectorField(fMagFieldFZB3);

  if(fChordFinderFZB3) delete fChordFinderFZB3;
  fIntgrDriverFZB3 = new G4MagInt_Driver(fMinStep,fStepperFZB3,fStepperFZB3->GetNumberOfVariables());
  fChordFinderFZB3 = new G4ChordFinder((G4MagneticField*) fMagFieldFZB3, fMinStep, fStepperFZB3);
  fLocalFieldManagerFZB3->SetChordFinder( fChordFinderFZB3 );

}

/////////////////////////////////////////////////////////////////////////////
void MolPolEMFieldSetup::UpdateFieldFZB4()
{

  fLocalFieldManagerFZB4->SetDetectorField(fMagFieldFZB4);

  if(fChordFinderFZB4) delete fChordFinderFZB4;
  fIntgrDriverFZB4 = new G4MagInt_Driver(fMinStep,fStepperFZB4,fStepperFZB4->GetNumberOfVariables());
  fChordFinderFZB4 = new G4ChordFinder((G4MagneticField*) fMagFieldFZB4, fMinStep, fStepperFZB4);
  fLocalFieldManagerFZB4->SetChordFinder( fChordFinderFZB4 );

}

void MolPolEMFieldSetup::UpdateFieldFZB5()
{

  fLocalFieldManagerFZB5->SetDetectorField(fMagFieldFZB5);

  if(fChordFinderFZB5) delete fChordFinderFZB5;
  fIntgrDriverFZB5 = new G4MagInt_Driver(fMinStep,fStepperFZB5,fStepperFZB5->GetNumberOfVariables());
  fChordFinderFZB5 = new G4ChordFinder((G4MagneticField*) fMagFieldFZB5, fMinStep, fStepperFZB5);
  fLocalFieldManagerFZB5->SetChordFinder( fChordFinderFZB5 );

}

void MolPolEMFieldSetup::UpdateFieldFZB6()
{

  fLocalFieldManagerFZB6->SetDetectorField(fMagFieldFZB6);

  if(fChordFinderFZB6) delete fChordFinderFZB6;
  fIntgrDriverFZB6 = new G4MagInt_Driver(fMinStep,fStepperFZB6,fStepperFZB6->GetNumberOfVariables());
  fChordFinderFZB6 = new G4ChordFinder((G4MagneticField*) fMagFieldFZB6, fMinStep, fStepperFZB6);
  fLocalFieldManagerFZB6->SetChordFinder( fChordFinderFZB6 );

}


/////////////////////////////////////////////////////////////////////////////
//
// Set stepper according to the stepper type
//


void MolPolEMFieldSetup::SetStepper()
{
  G4int nvar = 8;

  if(fStepper) delete fStepper;

  switch ( fStepperType )
    {
    case 0:
      fStepper = new G4ExplicitEuler( fEquation, nvar );
      break;
    case 1:
      fStepper = new G4ImplicitEuler( fEquation, nvar );
      break;
    case 2:
      fStepper = new G4SimpleRunge( fEquation, nvar );
      break;
    case 3:
      fStepper = new G4SimpleHeum( fEquation, nvar );
      break;
    case 4:
      fStepper = new G4ClassicalRK4( fEquation, nvar );
      break;
    case 5:
      fStepper = new G4CashKarpRKF45( fEquation, nvar );
      break;
    default: fStepper = 0;
    }
}


///////////////////////////////////////////////////////////////////////////////
// Current to Field Calculation (paprameters from Sasha)
// Return field at the pole tip (Quadrupole)

G4double MolPolEMFieldSetup::CalA2T(G4double current, G4int magnet)
{

  G4double gl1 = 0;
  G4double gl2 = 0;
  G4double gln = 0;
  G4double fld = 0;

  G4double cn = current / 300.;
  if(magnet == 1)
    {
      // Moller Quad Q1/MQO1H01/LARGE/new/white
      gl1 = (.0110605+5.33237*cn-.0142794*pow(cn,2)+.259313*pow(cn,3));
      gln = (gl1+0.0058174*pow(cn,4)-0.831887*pow(cn,5));
      fld = gln*10.*5.08/36.5723;
    }
  else if(magnet == 2)
    {
      // Moller Quad Q2/PATSY/MQM1H02/SMALL/RED
      gl1=(0.0196438+5.35443*cn+0.0297273*pow(cn,2)+0.103505*pow(cn,3));
      gln=(gl1-0.0449275*pow(cn,4)-0.211868*pow(cn,5));
      fld=gln*10.*5.08/44.76;
    }
  else if(magnet == 3)
    {
      // Moller Quad Q3/TESSA/MQO1H03/LARGE/BLUE
      gl1=(0.000632446+5.15178*cn-0.00262778*pow(cn,2));
      gl2=(-0.107635*pow(cn,3)+0.00209902*pow(cn,4));
      gln=(gl1+gl2-0.640635*pow(cn,5));
      fld=gln*10.*5.08/36.74 ;
    }
  else if(magnet == 4)
    {
      // Moller Quad Q4/FELICIA/MQO1H03A/LARGE/BLUE
      gl1=(0.0001732+5.2119*cn-0.000732518*pow(cn,2));
      gl2=(-0.133423*pow(cn,3)+0.000618402*pow(cn,4));
      gln=(gl1+gl2-0.647082*pow(cn,5));
      fld=gln*10.*5.08/36.50;
    }
  else if(magnet == 5)
    {
      // Moller Dipole LILLY/MMA1H01/Blue
      fld=(-0.39026E-04+0.027051*current-0.17799E-08*pow(current,2));
    }
  else
    {
      //wrong magnet setup
      fld = 0.0;
    }

  return fld * 0.1 * tesla;

}
