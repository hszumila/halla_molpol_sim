#ifndef MolPolEMFieldSetup_H
#define MolPolEMFieldSetup_H 1

#include "MolPolEMField.hh"
#include "MolPolQuad.hh"
#include "MolPolDipole.hh"
#include "MolPolSolenoid.hh"

class G4FieldManager;
class G4ChordFinder;
class G4EquationOfMotion;
class G4Mag_UsualEqRhs;
class G4Mag_EqRhs;
class G4EqMagElectricField;
class G4MagIntegratorStepper;
class G4MagInt_Driver;
class G4UniformMagField;
class MolPolEMFieldMessenger;

class MolPolEMFieldSetup
{
  //public:
  //Static method which returns the singleton pointer of this class.
  //    static MolPolEMFieldSetup* GetMolPolEMFieldSetup();
  /*
    private:
    static MolPolEMFieldSetup* fMolPolEMFieldSetup;
  */

public:
  MolPolEMFieldSetup() ;
  ~MolPolEMFieldSetup() ;

  void SetStepper();
  void UpdateField();

  void InitialseAll();

  inline  void SetStepperType( G4int val) { fStepperType = val ; }
  inline  G4int GetStepperType() {return fStepperType; }

  inline void SetMinStep(G4double val) { fMinStep = val ; }
  inline G4double GetMinStep() { return fMinStep ; }

  G4FieldManager* GetFieldManager(){return fFieldManager;}

  //Local field  FZB1
  void UpdateFieldFZB1();
  void SetBField3VFZB1(G4double fieldGradient);
  G4FieldManager* GetFieldManagerFZB1(){return fLocalFieldManagerFZB1;}

  //Local field  FZB2
  void UpdateFieldFZB2();
  void SetBField3VFZB2(G4double fieldGradient);
  G4FieldManager* GetFieldManagerFZB2(){return fLocalFieldManagerFZB2;}

  //Local field  FZB3
  void UpdateFieldFZB3();
  void SetBField3VFZB3(G4double fieldGradient);
  G4FieldManager* GetFieldManagerFZB3(){return fLocalFieldManagerFZB3;}

  //Local field  FZB4
  void UpdateFieldFZB4();
  void SetBField3VFZB4(G4double fieldGradient);
  G4FieldManager* GetFieldManagerFZB4(){return fLocalFieldManagerFZB4;}

  //Local field  FZB5
  void UpdateFieldFZB5();
  void SetBField3VFZB5(G4double fieldGradient);
  G4FieldManager* GetFieldManagerFZB5(){return fLocalFieldManagerFZB5;}

  //Local field  FZB6
  void UpdateFieldFZB6();
  void SetBField3VFZB6(G4double fieldGradient);
  G4FieldManager* GetFieldManagerFZB6(){return fLocalFieldManagerFZB6;}

  void UpdateConfiguration();

  //Messenger Values
  G4int fMagSourceMode;
  G4double                    fQ1A;
  G4double                    fQ2A;
  G4double                    fQ3A;
  G4double                    fQ4A;
  G4double                    fQ5A;
  G4double                    fQ6A;
  G4double                    fQ1T;
  G4double                    fQ2T;
  G4double                    fQ3T;
  G4double                    fQ4T;
  G4double                    fQ5T;
  G4double                    fQ6T;
  G4String                    fToscaQ1;
  G4String                    fToscaQ2;
  G4String                    fToscaDp;
  G4double                    fToscaQ1scale;
  G4double                    fToscaQ2scale;
  G4double                    fToscaQ3scale;
  G4double                    fToscaQ4scale;
  G4double                    fToscaDpscale;
  G4double                    fToscaQ1current;
  G4double                    fToscaQ2current;
  G4double                    fToscaQ3current;
  G4double                    fToscaQ4current;
  G4double                    fToscaDpcurrent;

private:
  MolPolEMField*              fEMfield;
  G4FieldManager*             fFieldManager;
  G4ChordFinder*              fChordFinder ;
  G4EqMagElectricField*       fEquation ;
  G4MagIntegratorStepper*     fStepper ;
  G4MagInt_Driver*            fIntgrDriver;
  G4MagInt_Driver*            fIntgrDriverFZB1;
  G4MagInt_Driver*            fIntgrDriverFZB2;
  G4MagInt_Driver*            fIntgrDriverFZB3;
  G4MagInt_Driver*            fIntgrDriverFZB4;
  G4MagInt_Driver*            fIntgrDriverFZB5;
  G4MagInt_Driver*            fIntgrDriverFZB6;

  MolPolEMFieldMessenger*     fFieldMessenger;

  G4int                       fStepperType ;
  G4double                    fMinStep;

  std::vector<G4String>       filelist;
  std::vector<G4double>       filescales;

  //Values set in EMFieldSetup.cc
  G4double                    KAPPA1 = 0.;
  G4double                    KAPPA2 = 0.;
  G4double                    KAPPA3 = 0.;
  G4double                    KAPPA4 = 0.;
  G4double                    DIPOLE = 0.;
  G4double                    SOLENOID = 0.;
  G4double                    ORIGINQ1 = 75.19 * cm;
  G4double                    ORIGINQ3 = 140.46 * cm;
  G4double                    ORIGINQ2 = 209.08 * cm;
  G4double                    ORIGINQ4 = 274.59 * cm;
  G4double                    ORIGINQ6 = 6.9 * cm;
  G4double                    ORIGIND = 423.4 * cm;
  G4double                    BORERADIUS = 5.08 * cm;

  G4String                    TOSCAQ1;
  G4String                    TOSCAQ2;
  G4String                    TOSCADP;

  G4double                    TOSCAQ1SCALE;
  G4double                    TOSCAQ1CURRENT;
  G4double                    TOSCAQ2SCALE;
  G4double                    TOSCAQ2CURRENT;
  G4double                    TOSCAQ3SCALE;
  G4double                    TOSCAQ3CURRENT;
  G4double                    TOSCAQ4SCALE;
  G4double                    TOSCAQ4CURRENT;
  G4double                    TOSCADPSCALE;
  G4double                    TOSCADPCURRENT;

  //for local field at FZB1 and FZB2
  //MolPolQuad*               fMagFieldFZB1 ;
  G4MagneticField*            fMagFieldFZB1 ;
  G4Mag_UsualEqRhs*           fEquationFZB1 ;
  G4ChordFinder*              fChordFinderFZB1 ;
  G4MagIntegratorStepper*     fStepperFZB1 ;
  G4FieldManager*             fLocalFieldManagerFZB1;

  //MolPolQuad*               fMagFieldFZB2 ;
  G4MagneticField*            fMagFieldFZB2 ;
  G4Mag_UsualEqRhs*           fEquationFZB2 ;
  G4ChordFinder*              fChordFinderFZB2 ;
  G4MagIntegratorStepper*     fStepperFZB2 ;
  G4FieldManager*             fLocalFieldManagerFZB2;

  //MolPolQuad*               fMagFieldFZB3 ;
  G4MagneticField*            fMagFieldFZB3 ;
  G4Mag_UsualEqRhs*           fEquationFZB3 ;
  G4ChordFinder*              fChordFinderFZB3 ;
  G4MagIntegratorStepper*     fStepperFZB3 ;
  G4FieldManager*             fLocalFieldManagerFZB3;

  //MolPolQuad*               fMagFieldFZB4 ;
  G4MagneticField*            fMagFieldFZB4 ;
  G4Mag_UsualEqRhs*           fEquationFZB4 ;
  G4ChordFinder*              fChordFinderFZB4 ;
  G4MagIntegratorStepper*     fStepperFZB4 ;
  G4FieldManager*             fLocalFieldManagerFZB4;

  //MolPolDipole*             fMagFieldFZB5 ;
  G4MagneticField*            fMagFieldFZB5 ;
  G4Mag_UsualEqRhs*           fEquationFZB5 ;
  G4ChordFinder*              fChordFinderFZB5 ;
  G4MagIntegratorStepper*     fStepperFZB5 ;
  G4FieldManager*             fLocalFieldManagerFZB5;

  //MolPolSolenoid*           fMagFieldFZB6 ;
  G4MagneticField*            fMagFieldFZB6 ;
  G4Mag_UsualEqRhs*           fEquationFZB6 ;
  G4ChordFinder*              fChordFinderFZB6 ;
  G4MagIntegratorStepper*     fStepperFZB6 ;
  G4FieldManager*             fLocalFieldManagerFZB6;

  G4double CalA2T(G4double current, G4int magnet);

};

#endif
