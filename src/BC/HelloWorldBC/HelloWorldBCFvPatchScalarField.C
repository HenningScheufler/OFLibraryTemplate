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

#include "HelloWorldBCFvPatchScalarField.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "surfaceFields.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

Foam::scalar Foam::HelloWorldBCFvPatchScalarField::t() const
{
    return db().time().timeOutputValue();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::HelloWorldBCFvPatchScalarField::
HelloWorldBCFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(p, iF),
    scalarData_(0),
    data_(Zero),
    fieldData_(p.size(), Zero),
    timeVsData_(),
    wordData_("wordDefault"),
    labelData_(-1),
    boolData_(false)
{
}


Foam::HelloWorldBCFvPatchScalarField::
HelloWorldBCFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchScalarField(p, iF),
    scalarData_(dict.get<scalar>("scalarData")),
    data_(dict.get<scalar>("data")),
    fieldData_("fieldData", dict, p.size()),
    timeVsData_(Function1<scalar>::New("timeVsData", dict)),
    wordData_(dict.getOrDefault<word>("wordName", "wordDefault")),
    labelData_(-1),
    boolData_(false)
{


    fixedValueFvPatchScalarField::evaluate();

    /*
    //Initialise with the value entry if evaluation is not possible
    fvPatchScalarField::operator=
    (
        scalarField("value", dict, p.size())
    );
    */
}


Foam::HelloWorldBCFvPatchScalarField::
HelloWorldBCFvPatchScalarField
(
    const HelloWorldBCFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchScalarField(ptf, p, iF, mapper),
    scalarData_(ptf.scalarData_),
    data_(ptf.data_),
    fieldData_(ptf.fieldData_, mapper),
    timeVsData_(ptf.timeVsData_.clone()),
    wordData_(ptf.wordData_),
    labelData_(-1),
    boolData_(ptf.boolData_)
{}


Foam::HelloWorldBCFvPatchScalarField::
HelloWorldBCFvPatchScalarField
(
    const HelloWorldBCFvPatchScalarField& ptf
)
:
    fixedValueFvPatchScalarField(ptf),
    scalarData_(ptf.scalarData_),
    data_(ptf.data_),
    fieldData_(ptf.fieldData_),
    timeVsData_(ptf.timeVsData_.clone()),
    wordData_(ptf.wordData_),
    labelData_(-1),
    boolData_(ptf.boolData_)
{}


Foam::HelloWorldBCFvPatchScalarField::
HelloWorldBCFvPatchScalarField
(
    const HelloWorldBCFvPatchScalarField& ptf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(ptf, iF),
    scalarData_(ptf.scalarData_),
    data_(ptf.data_),
    fieldData_(ptf.fieldData_),
    timeVsData_(ptf.timeVsData_.clone()),
    wordData_(ptf.wordData_),
    labelData_(-1),
    boolData_(ptf.boolData_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::HelloWorldBCFvPatchScalarField::autoMap
(
    const fvPatchFieldMapper& m
)
{
    fixedValueFvPatchScalarField::autoMap(m);
    fieldData_.autoMap(m);
}


void Foam::HelloWorldBCFvPatchScalarField::rmap
(
    const fvPatchScalarField& ptf,
    const labelList& addr
)
{
    fixedValueFvPatchScalarField::rmap(ptf, addr);

    const HelloWorldBCFvPatchScalarField& tiptf =
        refCast<const HelloWorldBCFvPatchScalarField>(ptf);

    fieldData_.rmap(tiptf.fieldData_, addr);
}


void Foam::HelloWorldBCFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    fixedValueFvPatchScalarField::operator==
    (
        data_
      + fieldData_
      + scalarData_*timeVsData_->value(t())
    );


    fixedValueFvPatchScalarField::updateCoeffs();
}


void Foam::HelloWorldBCFvPatchScalarField::write
(
    Ostream& os
) const
{
    fvPatchScalarField::write(os);
    os.writeEntry("scalarData", scalarData_);
    os.writeEntry("data", data_);
    fieldData_.writeEntry("fieldData", os);
    timeVsData_->writeData(os);
    os.writeEntry("wordData", wordData_);
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * Build Macro Function  * * * * * * * * * * * * //

namespace Foam
{
    makePatchTypeField
    (
        fvPatchScalarField,
        HelloWorldBCFvPatchScalarField
    );
}

// ************************************************************************* //
