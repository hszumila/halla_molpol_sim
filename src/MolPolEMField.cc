#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"

#include "MolPolEMField.hh"
#include "MolPolTOSCAField.hh"
#include <vector>

MolPolEMField* MolPolEMField::fObject = 0;

MolPolEMField* MolPolEMField::GetObject()
{
  if(!fObject) new MolPolEMField();
  return fObject;
}

MolPolEMField::MolPolEMField()
{
  //FIXME: What's the purpose of these fObjects -- not clear at this point...
  fObject = this;
  //EField3V.set(0,0,0);
  BField3V.set(0,0,0);
}

MolPolEMField::MolPolEMField( std::vector<G4String> files , std::vector<G4double> scales )
{
  fObject = this;

  if( !(sizeof(files) == sizeof(scales)) )
  G4Exception("MolPolEMField::MolPolEMField(files,scales)","vector sizes do not match", FatalException,"MolPolEMField | Global field vectors do not match in size...");

  //FIXME: This if fine but perhaps could be redone with boost iteration over both containers
  G4int size = sizeof(files);
  for(G4int i = 0; i < size; i++){
    AddNewField(files[i]);
    SetFieldScale(files[i],scales[i]);
    G4cout << "field file: " << files[i] << "sent to AddNewField()" << G4endl;
  }

}

MolPolEMField::~MolPolEMField()
{
  //FIXME: Make sure you populate this with the proper things so that we don't
  //       encounter and dangling problems or other memory issues.
}

////////////////////////////////////////////////////////////////////////////
//input Point[4] (x,y,z,t)
inline void MolPolEMField::GetFieldValue(const G4double Point[4],G4double *Bfield) const
{
  //////////////////////////////////////////////////////////
  //get BField
  if(this->bUseUniformBField){
    Bfield[0]=BField3V.x();
    Bfield[1]=BField3V.y();
    Bfield[2]=BField3V.z();
  }else{
    double pB[3],pPos[3]={Point[0]/cm,Point[1]/cm,Point[2]/cm};  //turn into cm
    for(int i=0;i<3;i++) Bfield[i]=0.0;  //reset
  }

  //////////////////////////////////////////////////////////
  //get EField,

  double  *Efield=&Bfield[3];
  if(this->bUseUniformEField)
    {
      Efield[0]=EField3V.x();
      Efield[1]=EField3V.y();
      Efield[2]=EField3V.z();
    }
  else
    {
      for(int i=0;i<3;i++) Efield[i]=0.;
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
void remollGlobalField::GetFieldValue( const G4double p[], G4double *resB) const
{
    G4double Bsum [__GLOBAL_NDIM], thisB[__GLOBAL_NDIM];

    for (int i = 0; i < __GLOBAL_NDIM; i++) {
        Bsum[i] = 0.0;
    }

    std::vector<MolPolTOSCAField*>::const_iterator it = fFields.begin();
    for (it = fFields.begin(); it != fFields.end(); it++) {
        (*it)->GetFieldValue(p, thisB);
        for (int i = 0; i < __GLOBAL_NDIM; i++) {
          Bsum[i] += thisB[i];
        }
    }

    for (int i = 0; i < __GLOBAL_NDIM; i++) {
        resB[i] = Bsum[i];
    }
}
*/
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void MolPolEMField::AddNewField(G4String& name)
{
    MolPolTOSCAField *thisfield = new MolPolTOSCAField(name);

    if (thisfield->IsInit()) {
        fFields.push_back(thisfield);

        // I don't know why it's necessary to do the following - SPR 1/24/13
        // Recreating the chord finder makes stepping bearable
        // in cases where you change the geometry.
        G4TransportationManager::GetTransportationManager()->GetFieldManager()->CreateChordFinder(this);

        G4cout << __FUNCTION__ << ": field " << name << " was added." << G4endl;

        // Add file data to output data stream
        // (MolPol)FIXME: I THINK THE FOLLOWING LINE SENDS INFORMATION TO ROOT FILE...
        // CONSIDER IMPLEMENTING IF THAT IS THE CASE.  THIS WOULD BE GOOD FOR GENERAL
        // SIMULATION RECORD KEEPING.
        //remollRunData *rd = remollRun::GetRunData();

        // (remoll)FIXME disabled TMD5 functionality as long as CentOS 7.2 is common
        // due to kernel bug when running singularity containers

        //TMD5 *md5 = TMD5::FileChecksum(name.data());

        //filedata_t fdata;

        //strcpy(fdata.filename, name.data());
        //strcpy(fdata.hashsum, "no hash" ); // md5->AsString() );

        //G4cout << "MD5 checksum " << md5->AsString() << G4endl;

        //delete md5;

        //struct stat fs;
        //stat(name.data(), &fs);
        //fdata.timestamp = TTimeStamp( fs.st_mtime );

        //G4cout << __FUNCTION__ << ": field timestamp = " << fdata.timestamp << G4endl;

        //rd->AddMagData(fdata);

    } else {
        G4cerr << "WARNING " << __FILE__ << " line " << __LINE__
            << ": field " << name << " was not initialized." << G4endl;
    }
}

MolPolTOSCAField* MolPolEMField::GetFieldByName(const G4String& name)
{
    std::vector<MolPolTOSCAField*>::iterator it = fFields.begin();
    while (it != fFields.end()) {
        if ((*it)->GetName() == name) break;
        it++;
    }

    if (it != fFields.end()) {
        return (*it);
    } else {
        G4cerr << "WARNING " << __FILE__ << " line " << __LINE__
            << ": field " << name << " not found." << G4endl;
        return NULL;
    }
}

void MolPolEMField::SetFieldScaleByString(G4String& name_scale)
{
  std::istringstream iss(name_scale);

  G4String name, scalestr;
  iss >> name;
  iss >> scalestr;

  G4double scaleval = atof(scalestr);
  SetFieldScale(name, scaleval);
}

void MolPolEMField::SetFieldScale(const G4String& name, G4double scale)
{
    MolPolTOSCAField *field = GetFieldByName(name);
    if (field) {
        field->SetFieldScale(scale);
    } else {
        G4cerr << "WARNING " << __FILE__ << " line " << __LINE__
            << ": field " << name << " scaling failed" << G4endl;
    }
}

void MolPolEMField::SetMagnetCurrentByString(G4String& name_scale)
{
  std::istringstream iss(name_scale);

  G4String name, scalestr, scaleunit;
  iss >> name;
  iss >> scalestr;
  iss >> scaleunit;

  if (scaleunit != "A") {
    // FIXME: less snark and more functionality?
    G4cerr << __FILE__ << " line " << __LINE__ <<  ":\n\tGraaaah - just put the current for " <<  name <<  " in amps..." << G4endl;
    exit(1);
  }

  G4double scaleval = atof(scalestr);
  SetMagnetCurrent(name, scaleval);
}

void MolPolEMField::SetMagnetCurrent(const G4String& name, G4double scale)
{
    MolPolTOSCAField *field = GetFieldByName(name);
    if (field) {
        field->SetMagnetCurrent(scale);
    } else {
        G4cerr << "WARNING " << __FILE__ << " line " << __LINE__
            << ": field " << name << " scaling failed" << G4endl;
    }
}
