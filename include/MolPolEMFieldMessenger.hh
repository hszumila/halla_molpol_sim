#ifndef MolPolEMFieldMessenger_h
#define MolPolEMFieldMessenger_h 1

#include "G4UImessenger.hh"

class MolPolEMFieldSetup;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MolPolEMFieldMessenger: public G4UImessenger
{
  public:
    MolPolEMFieldMessenger(MolPolEMFieldSetup* );
    virtual ~MolPolEMFieldMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:

    MolPolEMFieldSetup*        fEMfieldSetup;

    G4UIdirectory*             fFieldDir;

    G4UIcmdWithAnInteger*      fMagSourceCmd;
    G4UIcmdWithADouble*        fQ1ACmd;
    G4UIcmdWithADouble*        fQ2ACmd;
    G4UIcmdWithADouble*        fQ3ACmd;
    G4UIcmdWithADouble*        fQ4ACmd;
    G4UIcmdWithADouble*        fQ5ACmd;
    G4UIcmdWithADouble*        fQ6ACmd;

    G4UIcmdWithADouble*        fQ1TCmd;
    G4UIcmdWithADouble*        fQ2TCmd;
    G4UIcmdWithADouble*        fQ3TCmd;
    G4UIcmdWithADouble*        fQ4TCmd;
    G4UIcmdWithADouble*        fQ5TCmd;
    G4UIcmdWithADouble*        fQ6TCmd;

    G4UIcmdWithAString*        fToscaQ1Cmd;
    G4UIcmdWithAString*        fToscaQ2Cmd;
    G4UIcmdWithAString*        fToscaDpCmd;

    G4UIcmdWithADouble*        fToscaQ1ScaleCmd;
    G4UIcmdWithADouble*        fToscaQ1CurrentCmd;
    G4UIcmdWithADouble*        fToscaQ2ScaleCmd;
    G4UIcmdWithADouble*        fToscaQ2CurrentCmd;
    G4UIcmdWithADouble*        fToscaQ3ScaleCmd;
    G4UIcmdWithADouble*        fToscaQ3CurrentCmd;
    G4UIcmdWithADouble*        fToscaQ4ScaleCmd;
    G4UIcmdWithADouble*        fToscaQ4CurrentCmd;
    G4UIcmdWithADouble*        fToscaDpScaleCmd;
    G4UIcmdWithADouble*        fToscaDpCurrentCmd;

    G4UIcmdWithoutParameter*   fUpdateCmd;

};

#endif
