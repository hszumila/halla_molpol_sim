#include "MolPolQuad.hh"
#include "G4RotationMatrix.hh"

static G4RotationMatrix IdentityMatrix;

MolPolQuad::MolPolQuad(G4double pGradient, G4ThreeVector pOrigin, G4RotationMatrix* pMatrix, G4double pRadius)
{
  fGradient    = pGradient ;
  fOrigin      = pOrigin ;
  fpMatrix     = pMatrix ;
  fRadius      = pRadius;
}


/////////////////////////////////////////////////////////////////////////

void MolPolQuad::UpdateQuad(G4double pGradient, G4ThreeVector pOrigin, G4RotationMatrix* pMatrix, G4double pRadius)
{
  fGradient    = pGradient ;
  fOrigin      = pOrigin ;
  fpMatrix     = pMatrix ;
  fRadius      = pRadius;
}

/////////////////////////////////////////////////////////////////////////
MolPolQuad::~MolPolQuad()
{
}

////////////////////////////////////////////////////////////////////////
//  Allow displaced origin and rotation
//  Extensions by Björn Riese (GSI)

void MolPolQuad::GetFieldValue( const G4double y[4], G4double B[3]  ) const
{

  B[0] = 0;
  B[1] = 0;
  B[2] = 0;

  G4ThreeVector r_global= G4ThreeVector
    (y[0] - fOrigin.x(),
     y[1] - fOrigin.y(),
     y[2] - fOrigin.z());

  G4ThreeVector r_local = G4ThreeVector
    (fpMatrix->colX() * r_global,
     fpMatrix->colY() * r_global,
     fpMatrix->colZ() * r_global);

  G4ThreeVector B_local = G4ThreeVector
    (fGradient * r_local.y(),
     fGradient * r_local.x(),
     0);

  G4ThreeVector B_global = G4ThreeVector
    (fpMatrix->rowX() * B_local,
     fpMatrix->rowY() * B_local,
     fpMatrix->rowZ() * B_local);

  G4double rquad = (r_global.x() * r_global.x() + r_global.y() * r_global.y());
  if(rquad < (fRadius * fRadius))
    {
      B[0] = -1.0 * B_global.x() ;
      B[1] = -1.0 * B_global.y() ;
      B[2] = B_global.z() ;
    }
}
