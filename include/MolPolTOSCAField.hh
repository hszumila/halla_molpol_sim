/* MolPolToscaField adopted from remoll */

#ifndef MolPolTOSCAField_H
#define MolPolTOSCAField_H

#include <vector>

#define __NDIM 3

#include "G4String.hh"
#include "G4MagneticField.hh"

class MolPolTOSCAField : public G4MagneticField {
    /*!
     * Moller spectrometer magnetic field class
     *
     *
     * Units are meters, degrees, and Tesla
     * Coordinate ordering will be r, phi, z
     * We will only deal with phi interval [-pi,pi]
     *
     * Field maps are of the form
     *
     * #Rpoints		rmin	rmax
     * #Phipoints	phimin	phimax
     * #Zpoints		zmin	zmax
     * # xtants (7 for septant form, 1 for full geometry)
     * r   phi   z    br   bphi   bz
     * ......
     *
     */

    public:
	MolPolTOSCAField( G4String );
	virtual ~MolPolTOSCAField();

	void GetFieldValue( const   G4double Point[4], G4double *Bfield ) const;

	void InitializeGrid();
	void ReadFieldMap();

	void SetFieldScale(G4double s);
	void SetMagnetCurrent(G4double s);

	void SetZoffset(G4double z){ fZoffset= z; }

	G4String GetName();

	enum Coord_t { kR, kPhi, kZ };

	G4bool IsInit(){ return fInit; }

    private:
	G4String fFilename;

	G4int fN[__NDIM];
	G4double fMin[__NDIM], fMax[__NDIM];
	G4double fFileMin[__NDIM], fFileMax[__NDIM];

	G4int fNxtant; // Number of *tants (septants, or whatever)
	G4double fPhi0, fPhiLow, fxtantSize;

	// Storage space for the table
	std::vector< std::vector< std::vector< G4double > > > fBFieldData[__NDIM];

	G4double fZoffset, fPhiOffset;
	G4double fZMapOffset, fPhiMapOffset;

	G4double fFieldScale; // Scale overall field by this amount
	G4double fMagCurrent0; // Scale overall field by this amount

	G4bool fInit;
};

#endif
