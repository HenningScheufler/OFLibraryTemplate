/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2021 AUTHOR,AFFILIATION
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "writeAvg.H"
#include "Time.H"
#include "fvMesh.H"
#include "addToRunTimeSelectionTable.H"
#include "volFields.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(writeAvg, 0);
    addToRunTimeSelectionTable(functionObject, writeAvg, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::writeAvg::writeAvg
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    fvMeshFunctionObject(name, runTime, dict),
    writeFile(obr_, name, typeName, dict),
    fieldName_(dict.get<word>("fieldName"))
{
    read(dict);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::writeAvg::read(const dictionary& dict)
{
    return true;
}


bool Foam::functionObjects::writeAvg::execute()
{
    return true;
}


bool Foam::functionObjects::writeAvg::end()
{
    return true;
}


bool Foam::functionObjects::writeAvg::write()
{    
    const volScalarField& field = lookupObject<volScalarField>(fieldName_);

    scalar avgValue = gAverage(field.internalField());

    Log << type() << " " << name() << " write:" << nl
        << "    avgValue is  " << avgValue << endl;

    if (Pstream::master())
    {
        writeCurrentTime(file());

        file()
            << token::TAB << avgValue
            << endl;
    }


    return true;
}


// ************************************************************************* //
