#include "MolPolEMFieldMessenger.hh"

#include "MolPolEMFieldSetup.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
MolPolEMFieldMessenger::MolPolEMFieldMessenger(MolPolEMFieldSetup* fieldSetup)
  : G4UImessenger(),
    fEMfieldSetup(fieldSetup),
    fFieldDir(0)
{
  fFieldDir = new G4UIdirectory("/field/");
  fFieldDir->SetGuidance("MolPolEM field tracking control.");

  fMagSourceCmd = new G4UIcmdWithAnInteger("/field/MagSourceMode", this);
  fMagSourceCmd->SetGuidance("Set source mode for mag field setting");
  fMagSourceCmd->SetParameterName("magsource", false);
  fMagSourceCmd->SetDefaultValue(1);

  fQ1ACmd = new G4UIcmdWithADouble("/field/setQ1A", this);
  fQ1ACmd->SetGuidance("Set Q1 current");
  fQ1ACmd->SetParameterName("Q1A", false);

  fQ2ACmd = new G4UIcmdWithADouble("/field/setQ2A", this);
  fQ2ACmd->SetGuidance("Set Q2 current");
  fQ2ACmd->SetParameterName("Q2A", false);

  fQ3ACmd = new G4UIcmdWithADouble("/field/setQ3A", this);
  fQ3ACmd->SetGuidance("Set Q3 current");
  fQ3ACmd->SetParameterName("Q3A", false);

  fQ4ACmd = new G4UIcmdWithADouble("/field/setQ4A", this);
  fQ4ACmd->SetGuidance("Set Q4 current");
  fQ4ACmd->SetParameterName("Q4A", false);

  fQ5ACmd = new G4UIcmdWithADouble("/field/setQ5A", this);
  fQ5ACmd->SetGuidance("Set Dipole current");
  fQ5ACmd->SetParameterName("Q5A", false);

  fQ6ACmd = new G4UIcmdWithADouble("/field/setQ6A", this);
  fQ6ACmd->SetGuidance("Set Dipole current");
  fQ6ACmd->SetParameterName("Q6A", false);

  fQ1TCmd = new G4UIcmdWithADouble("/field/setQ1T", this);
  fQ1TCmd->SetGuidance("Set Q1 field in Tesla");
  fQ1TCmd->SetParameterName("Q1T", false);

  fQ2TCmd = new G4UIcmdWithADouble("/field/setQ2T", this);
  fQ2TCmd->SetGuidance("Set Q2 field in Tesla" );
  fQ2TCmd->SetParameterName("Q2T", false);

  fQ3TCmd = new G4UIcmdWithADouble("/field/setQ3T", this);
  fQ3TCmd->SetGuidance("Set Q3 field in Tesla");
  fQ3TCmd->SetParameterName("Q3T", false);

  fQ4TCmd = new G4UIcmdWithADouble("/field/setQ4T", this);
  fQ4TCmd->SetGuidance("Set Q4 field in Tesla");
  fQ4TCmd->SetParameterName("Q4T", false);

  fQ5TCmd = new G4UIcmdWithADouble("/field/setQ5T", this);
  fQ5TCmd->SetGuidance("Set Dipole field in Tesla");
  fQ5TCmd->SetParameterName("Q5T", false);

  fQ6TCmd = new G4UIcmdWithADouble("/field/setQ6T", this);
  fQ6TCmd->SetGuidance("Set Holding field in Tesla");
  fQ6TCmd->SetParameterName("Q6T", false);

  // =^..^=   =^..^=   =^..^=    =^..^=    =^..^=    =^..^=    =^..^=    =^..^=
  // TODO: COULD PROBABLY MAKE THE TOSCA MORE GENERAL LIKE IN REMOLL (MAYBE?)
  // CAN COME BACK LATER AND DO THAT IF WANTED.

  //TOSCA Q1 :: For Q1/3/4 Quad magnets
  fToscaQ1Cmd = new G4UIcmdWithAString("/field/setToscaQ1",this);
  fToscaQ1Cmd->SetGuidance("Relative path location for TOSCA file for Q1/Q3/Q4 Quads");
  fToscaQ1Cmd->SetParameterName("ToscaQ1",false);

  //TOSCA Q2 :: For the Q2 quad magnet
  fToscaQ2Cmd = new G4UIcmdWithAString("/field/setToscaQ2",this);
  fToscaQ2Cmd->SetGuidance("Relative path location for TOSCA file for Q2 Quad");
  fToscaQ2Cmd->SetParameterName("ToscaQ2",false);

  //TOSCA Dp :: Dipole
  fToscaDpCmd = new G4UIcmdWithAString("/field/setToscaDp",this);
  fToscaDpCmd->SetGuidance("Relative path location for TOSCA file for Dipole/Lilly");
  fToscaDpCmd->SetParameterName("ToscaDp",false);

  //TOSCA Scales/Current Settings
  fToscaQ1ScaleCmd = new G4UIcmdWithADouble("/field/setToscaQ1Scale",this);
  fToscaQ1ScaleCmd->SetGuidance("Set TOSCA Q1 mag field map scale [scale (decimal)]");
  fToscaQ1ScaleCmd->SetParameterName("ToscaQ1FieldScale",false);

  fToscaQ1CurrentCmd = new G4UIcmdWithADouble("/field/setToscaQ1Current",this);
  fToscaQ1CurrentCmd->SetGuidance("Set TOSCA Q1 mag field current in AMPS [value (decimal)]");
  fToscaQ1CurrentCmd->SetParameterName("ToscaQ1FieldCurrent",false);

  fToscaQ2ScaleCmd = new G4UIcmdWithADouble("/field/setToscaQ2Scale",this);
  fToscaQ2ScaleCmd->SetGuidance("Set TOSCA Q2 mag field map scale [scale (decimal)]");
  fToscaQ2ScaleCmd->SetParameterName("ToscaQ2FieldScale",false);

  fToscaQ2CurrentCmd = new G4UIcmdWithADouble("/field/setToscaQ2Current",this);
  fToscaQ2CurrentCmd->SetGuidance("Set TOSCA Q2 mag field current in AMPS [value (decimal)]");
  fToscaQ2CurrentCmd->SetParameterName("ToscaQ2FieldCurrent",false);

  fToscaQ3ScaleCmd = new G4UIcmdWithADouble("/field/setToscaQ3Scale",this);
  fToscaQ3ScaleCmd->SetGuidance("Set TOSCA Q3 mag field map scale [scale (decimal)]");
  fToscaQ3ScaleCmd->SetParameterName("ToscaQ3FieldScale",false);

  fToscaQ3CurrentCmd = new G4UIcmdWithADouble("/field/setToscaQ3Current",this);
  fToscaQ3CurrentCmd->SetGuidance("Set TOSCA Q3 mag field current in AMPS [value (decimal)]");
  fToscaQ3CurrentCmd->SetParameterName("ToscaQ3FieldCurrent",false);

  fToscaQ4ScaleCmd = new G4UIcmdWithADouble("/field/setToscaQ4Scale",this);
  fToscaQ4ScaleCmd->SetGuidance("Set TOSCA Q4 mag field map scale [scale (decimal)]");
  fToscaQ4ScaleCmd->SetParameterName("ToscaQ4FieldScale",false);

  fToscaQ4CurrentCmd = new G4UIcmdWithADouble("/field/setToscaQ4Current",this);
  fToscaQ4CurrentCmd->SetGuidance("Set TOSCA Q4 mag field current in AMPS [value (decimal)]");
  fToscaQ4CurrentCmd->SetParameterName("ToscaQ4FieldCurrent",false);

  fToscaDpScaleCmd = new G4UIcmdWithADouble("/field/setToscaDpScale",this);
  fToscaDpScaleCmd->SetGuidance("Set TOSCA Dp mag field map scale [scale (decimal)]");
  fToscaDpScaleCmd->SetParameterName("ToscaDpFieldScale",false);

  fToscaDpCurrentCmd = new G4UIcmdWithADouble("/field/setToscaDpCurrent",this);
  fToscaDpCurrentCmd->SetGuidance("Set TOSCA Dp mag field current in AMPS [value (decimal)]");
  fToscaDpCurrentCmd->SetParameterName("ToscaDpFieldCurrent",false);

  fUpdateCmd = new G4UIcmdWithoutParameter("/field/update",this);
  fUpdateCmd->SetGuidance("This command MUST be applied after setting field values ");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MolPolEMFieldMessenger::~MolPolEMFieldMessenger()
{
  delete fFieldDir;
  delete fUpdateCmd;
  delete fMagSourceCmd;
  delete fQ1ACmd;
  delete fQ2ACmd;
  delete fQ3ACmd;
  delete fQ4ACmd;
  delete fQ5ACmd;
  delete fQ6ACmd;
  delete fQ1TCmd;
  delete fQ2TCmd;
  delete fQ3TCmd;
  delete fQ4TCmd;
  delete fQ5TCmd;
  delete fQ6TCmd;
  delete fToscaQ1Cmd;
  delete fToscaQ2Cmd;
  delete fToscaDpCmd;
  delete fToscaQ1ScaleCmd;
  delete fToscaQ1CurrentCmd;
  delete fToscaQ2ScaleCmd;
  delete fToscaQ2CurrentCmd;
  delete fToscaQ3ScaleCmd;
  delete fToscaQ3CurrentCmd;
  delete fToscaQ4ScaleCmd;
  delete fToscaQ4CurrentCmd;
  delete fToscaDpScaleCmd;
  delete fToscaDpCurrentCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MolPolEMFieldMessenger::SetNewValue( G4UIcommand* cmd, G4String newValue)
{

  if( cmd == fMagSourceCmd ){
    G4double x = fMagSourceCmd->GetNewIntValue(newValue);
    fEMfieldSetup->fMagSourceMode = x;
  }else if( cmd == fQ1ACmd ){
    G4double x = fQ1ACmd->GetNewDoubleValue(newValue);
    fEMfieldSetup->fQ1A = x;
  }else if( cmd == fQ2ACmd ){
    G4double x = fQ2ACmd->GetNewDoubleValue(newValue);
    fEMfieldSetup->fQ2A = x;
  }else if( cmd == fQ3ACmd ){
    G4double x = fQ3ACmd->GetNewDoubleValue(newValue);
    fEMfieldSetup->fQ3A = x;
  }else if( cmd == fQ4ACmd ){
    G4double x = fQ4ACmd->GetNewDoubleValue(newValue);
    fEMfieldSetup->fQ4A = x;
  }else if( cmd == fQ5ACmd ){
    G4double x = fQ5ACmd->GetNewDoubleValue(newValue);
    fEMfieldSetup->fQ5A = x;
  }else if( cmd == fQ6ACmd ){
    G4double x = fQ6ACmd->GetNewDoubleValue(newValue);
    fEMfieldSetup->fQ6A = x;
  }else if( cmd == fQ1TCmd ){
    G4double x = fQ1TCmd->GetNewDoubleValue(newValue);
    fEMfieldSetup->fQ1T = x;
  }else if( cmd == fQ2TCmd ){
    G4double x = fQ2TCmd->GetNewDoubleValue(newValue);
    fEMfieldSetup->fQ2T = x;
  }else if( cmd == fQ3TCmd ){
    G4double x = fQ3TCmd->GetNewDoubleValue(newValue);
    fEMfieldSetup->fQ3T = x;
  }else if( cmd == fQ4TCmd ){
    G4double x = fQ4TCmd->GetNewDoubleValue(newValue);
    fEMfieldSetup->fQ4T = x;
  }else if( cmd == fQ5TCmd ){
    G4double x = fQ5TCmd->GetNewDoubleValue(newValue);
    fEMfieldSetup->fQ5T = x;
  }else if( cmd == fQ6TCmd ){
    G4double x = fQ6TCmd->GetNewDoubleValue(newValue);
    fEMfieldSetup->fQ6T = x;
  }else if( cmd == fToscaQ1Cmd ){
    fEMfieldSetup->fToscaQ1 = newValue;
  }else if( cmd == fToscaQ2Cmd ){
    fEMfieldSetup->fToscaQ2 = newValue;
  }else if( cmd == fToscaDpCmd ){
    fEMfieldSetup->fToscaDp = newValue;
  }else if( cmd == fToscaQ1ScaleCmd ){
    G4double x = fToscaQ1ScaleCmd->GetNewDoubleValue(newValue);
    fEMfieldSetup->fToscaQ1scale = x;
    G4cout << "fToscaQ1ScaleCmd: " << x << G4endl;
  }else if( cmd == fToscaQ1CurrentCmd ){
    fEMfieldSetup->fToscaQ1current = fToscaQ1CurrentCmd->GetNewDoubleValue(newValue);
  }else if( cmd == fToscaQ2ScaleCmd ){
    fEMfieldSetup->fToscaQ2scale = fToscaQ2ScaleCmd->GetNewDoubleValue(newValue);
  }else if( cmd == fToscaQ2CurrentCmd ){
    fEMfieldSetup->fToscaQ2current = fToscaQ2CurrentCmd->GetNewDoubleValue(newValue);
  }else if( cmd == fToscaQ3ScaleCmd ){
    fEMfieldSetup->fToscaQ3scale = fToscaQ3ScaleCmd->GetNewDoubleValue(newValue);
  }else if( cmd == fToscaQ3CurrentCmd ){
    fEMfieldSetup->fToscaQ3current = fToscaQ3CurrentCmd->GetNewDoubleValue(newValue);
  }else if( cmd == fToscaQ4ScaleCmd ){
    fEMfieldSetup->fToscaQ4scale = fToscaQ4ScaleCmd->GetNewDoubleValue(newValue);
  }else if( cmd == fToscaQ4CurrentCmd ){
    fEMfieldSetup->fToscaQ4current = fToscaQ4CurrentCmd->GetNewDoubleValue(newValue);
  }else if( cmd == fToscaDpScaleCmd ){
    fEMfieldSetup->fToscaDpscale = fToscaDpScaleCmd->GetNewDoubleValue(newValue);
  }else if( cmd == fToscaDpCurrentCmd ){
    fEMfieldSetup->fToscaDpcurrent = fToscaDpCurrentCmd->GetNewDoubleValue(newValue);
  }else if( cmd == fUpdateCmd ){
    G4cout << "Updating magnetic field configuration... " << G4endl;
    fEMfieldSetup->UpdateConfiguration();
  }else{
    G4cout << __PRETTY_FUNCTION__ << " at line " << __LINE__ << G4endl;
    G4cerr << "Don't know this command :" << cmd << G4endl;
  }

  /*
    if( command == fStepperCmd )
    fEMfieldSetup->SetStepperType(fStepperCmd->GetNewIntValue(newValue));
    if( command == fUpdateCmd )
    fEMfieldSetup->CreateStepperAndChordFinder();
    if( command == fMagFieldCmd )
    fEMfieldSetup->SetFieldValue(fMagFieldCmd->GetNewDoubleValue(newValue));
    if( command == fMinStepCmd )
    fEMfieldSetup->SetMinStep(fMinStepCmd->GetNewDoubleValue(newValue));
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
