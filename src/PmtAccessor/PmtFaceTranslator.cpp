/*
 * Copyright (c) 2000-2009, Eastman Kodak Company
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification,are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice, 
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the 
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Eastman Kodak Company nor the names of its 
 *       contributors may be used to endorse or promote products derived from 
 *       this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 * 
 * Creation date: 1/26/2009
 *
 * Original Author: 
 * Sam Fryer <samuel.fryer@kodak.com>
 *
*/

#include "PmtTranslator.h"
#include "PmtFaceTranslator.h"
#include "PmtAtom.h"
#include "PmtFaceAtoms.h"
#include "PmtCompositeMetadata.h"

#include <iostream>
#include <fstream>
#include <strstream>

std::vector<PmtFacePoint> getFacePts(SPmtString spts)
{
	std::vector<PmtFacePoint> points;
	SPmtString::size_type z = SPmtString::npos;
	SPmtString::size_type oldz = 0;
	bool inY = false;
	PmtFaceFloatPtr x;
	PmtFaceFloatPtr y;

	while((z = spts.find_first_of(", ",oldz)) != SPmtString::npos)
	{
		double val = atof(spts.substr(oldz,z-oldz).c_str());
		if (!inY)
		{
			x = new PmtFaceFloat(val);
			inY = true;
		}
		else
		{
			y = new PmtFaceFloat(val);
			points.push_back(PmtFacePoint(x,y));
			inY = false;
		}
		oldz = z+1;
	}
	
	// Gotta catch the last point!
	if ((inY)&&(oldz < spts.length()))
	{
		double val = atof(spts.substr(oldz).c_str());
		y = new PmtFaceFloat(val);
		points.push_back(PmtFacePoint(x,y));
	}

	return points;
}





#define MY_MAX_FACE_NUM 0x000FFFFF
void PmtFaceTranslator::assign(PmtMetadata * md, void * &vformatValue,
                            unsigned int &fvCount, bool valueToMetadata)
{
	PmtCompositeMetadata *  container = NULL;
	if ((md != NULL)&&(md->getMDType() == PMT_MD_TYPE_COMPOSITE))
		container = (PmtCompositeMetadata *)md;
	if ((container != NULL)&&(valueToMetadata))
	{
		unsigned long long readsize = 0;
		SPmtString tag;
		PmtMetadataIterator iter = container->getMetadata("Object",false);
		unsigned int facecount = iter.size();
		while (readsize<fvCount)
		{
			PmtContainerAtomPtr atom = (PmtContainerAtom*)PmtAtom::GetNextAtom(((unsigned char *)vformatValue)+readsize,fvCount-readsize);
			if (atom)
			{
				unsigned int featureCount = 0;
				atom->readAtom();
				readsize += atom->getAtomSize();
				readsize += 8; // atoms don't take into account their size/name.

				// Note: we want to force a creation of metadata ptrs here. 
				PmtCompositeMetadataPtr face = (PmtCompositeMetadata *)container->modelGroup()->getMetadatum("Object",++facecount).ptr();
				PmtMetadataPtr m = face->getMetadatum("Type");
				if (m) m->setValueStr(EK_STR2WIDE("face"));
				std::vector < PmtAtomPtr >  atoms = atom->getAtoms("ffob.obID");
				if(atoms.size() > 0)
				{
					m = face->getMetadatum("ID");
					m->setValueStr(EK_STR2WIDE(atoms[0]->getMetadata("obID",tag)));
				}
				atoms = atom->getAtoms("ffob.obNa");
				if(atoms.size() > 0)
				{
					m = face->getMetadatum("Name");
					m->setValueStr(EK_STR2WIDE(atoms[0]->getMetadata("obNa",tag)));
				}
				atoms = atom->getAtoms("ffob.obEv");
				if(atoms.size() > 0)
				{
					
				}
				atoms = atom->getAtoms("ffob.leye");
				if(atoms.size() > 0)
				{
					PmtCompositeMetadataPtr feature = (PmtCompositeMetadata *)face->modelGroup()->getMetadatum("Feature",++featureCount).ptr();
					m = feature->getMetadatum("Type");
					m->setValueStr(EK_STR2WIDE("LeftEye"));
					m = feature->getMetadatum("Data");
					m->setValueStr(EK_STR2WIDE(atoms[0]->getMetadata("leye",tag)));
				}
				atoms = atom->getAtoms("ffob.reye");
				if(atoms.size() > 0)
				{
					PmtCompositeMetadataPtr feature = (PmtCompositeMetadata *)face->modelGroup()->getMetadatum("Feature",++featureCount).ptr();
					m = feature->getMetadatum("Type");
					m->setValueStr(EK_STR2WIDE("RightEye"));
					m = feature->getMetadatum("Data");
					m->setValueStr(EK_STR2WIDE(atoms[0]->getMetadata("reye",tag)));
				}
				atoms = atom->getAtoms("ffob.fout");
				if(atoms.size() > 0)
				{
					PmtCompositeMetadataPtr feature = (PmtCompositeMetadata *)face->modelGroup()->getMetadatum("Feature",++featureCount).ptr();
					m = feature->getMetadatum("Type");
					m->setValueStr(EK_STR2WIDE("Outline"));
					m = feature->getMetadatum("Data");
					m->setValueStr(EK_STR2WIDE(atoms[0]->getMetadata("fout",tag)));
				}
				atoms = atom->getAtoms("ffob.LOEC");
				if(atoms.size() > 0)
				{
					PmtCompositeMetadataPtr feature = (PmtCompositeMetadata *)face->modelGroup()->getMetadatum("Feature",++featureCount).ptr();
					m = feature->getMetadatum("Type");
					m->setValueStr(EK_STR2WIDE("LeftOuterEyeCoordinate"));
					m = feature->getMetadatum("Data");
					m->setValueStr(EK_STR2WIDE(atoms[0]->getMetadata("LOEC",tag)));
				}
				atoms = atom->getAtoms("ffob.LIEC");
				if(atoms.size() > 0)
				{
					PmtCompositeMetadataPtr feature = (PmtCompositeMetadata *)face->modelGroup()->getMetadatum("Feature",++featureCount).ptr();
					m = feature->getMetadatum("Type");
					m->setValueStr(EK_STR2WIDE("LeftInnerEyeCoordinate"));
					m = feature->getMetadatum("Data");
					m->setValueStr(EK_STR2WIDE(atoms[0]->getMetadata("LIEC",tag)));
				}
				atoms = atom->getAtoms("ffob.ROEC");
				if(atoms.size() > 0)
				{
					PmtCompositeMetadataPtr feature = (PmtCompositeMetadata *)face->modelGroup()->getMetadatum("Feature",++featureCount).ptr();
					m = feature->getMetadatum("Type");
					m->setValueStr(EK_STR2WIDE("RightOuterEyeCoordinate"));
					m = feature->getMetadatum("Data");
					m->setValueStr(EK_STR2WIDE(atoms[0]->getMetadata("ROEC",tag)));
				}
				atoms = atom->getAtoms("ffob.RIEC");
				if(atoms.size() > 0)
				{
					PmtCompositeMetadataPtr feature = (PmtCompositeMetadata *)face->modelGroup()->getMetadatum("Feature",++featureCount).ptr();
					m = feature->getMetadatum("Type");
					m->setValueStr(EK_STR2WIDE("RightInnerEyeCoordinate"));
					m = feature->getMetadatum("Data");
					m->setValueStr(EK_STR2WIDE(atoms[0]->getMetadata("RIEC",tag)));
				}
				atoms = atom->getAtoms("ffob.LMCP");
				if(atoms.size() > 0)
				{
					PmtCompositeMetadataPtr feature = (PmtCompositeMetadata *)face->modelGroup()->getMetadatum("Feature",++featureCount).ptr();
					m = feature->getMetadatum("Type");
					m->setValueStr(EK_STR2WIDE("LeftMouthCenterPoint"));
					m = feature->getMetadatum("Data");
					m->setValueStr(EK_STR2WIDE(atoms[0]->getMetadata("LMCP",tag)));
				}
				atoms = atom->getAtoms("ffob.RMCP");
				if(atoms.size() > 0)
				{
					PmtCompositeMetadataPtr feature = (PmtCompositeMetadata *)face->modelGroup()->getMetadatum("Feature",++featureCount).ptr();
					m = feature->getMetadatum("Type");
					m->setValueStr(EK_STR2WIDE("RightMouthCenterPoint"));
					m = feature->getMetadatum("Data");
					m->setValueStr(EK_STR2WIDE(atoms[0]->getMetadata("RMCP",tag)));
				}
				atoms = atom->getAtoms("ffob.FPPA");
				if(atoms.size() > 0)
				{
					PmtCompositeMetadataPtr feature = (PmtCompositeMetadata *)face->modelGroup()->getMetadatum("Feature",++featureCount).ptr();
					m = feature->getMetadatum("Type");
					m->setValueStr(EK_STR2WIDE("FacePosePitchAngle"));
					m = feature->getMetadatum("Data");
					m->setValueStr(EK_STR2WIDE(atoms[0]->getMetadata("FPPA",tag)));
				}
				atoms = atom->getAtoms("ffob.FPYA");
				if(atoms.size() > 0)
				{
					PmtCompositeMetadataPtr feature = (PmtCompositeMetadata *)face->modelGroup()->getMetadatum("Feature",++featureCount).ptr();
					m = feature->getMetadatum("Type");
					m->setValueStr(EK_STR2WIDE("FacePoseYawAngle"));
					m = feature->getMetadatum("Data");
					m->setValueStr(EK_STR2WIDE(atoms[0]->getMetadata("FPYA",tag)));
				}
				atoms = atom->getAtoms("ffob.FPRA");
				if(atoms.size() > 0)
				{
					PmtCompositeMetadataPtr feature = (PmtCompositeMetadata *)face->modelGroup()->getMetadatum("Feature",++featureCount).ptr();
					m = feature->getMetadatum("Type");
					m->setValueStr(EK_STR2WIDE("FacePoseRotationAngle"));
					m = feature->getMetadatum("Data");
					m->setValueStr(EK_STR2WIDE(atoms[0]->getMetadata("FPRA",tag)));
				}
				atoms = atom->getAtoms("ffob.FPCL");
				if(atoms.size() > 0)
				{
					PmtCompositeMetadataPtr feature = (PmtCompositeMetadata *)face->modelGroup()->getMetadatum("Feature",++featureCount).ptr();
					m = feature->getMetadatum("Type");
					m->setValueStr(EK_STR2WIDE("FacePoseConfidenceLevel"));
					m = feature->getMetadatum("Data");
					m->setValueStr(EK_STR2WIDE(atoms[0]->getMetadata("FPCL",tag)));
				}
				atoms = atom->getAtoms("ffob.uCON");
				if(atoms.size() > 0)
				{
					PmtCompositeMetadataPtr feature = (PmtCompositeMetadata *)face->modelGroup()->getMetadatum("Feature",++featureCount).ptr();
					m = feature->getMetadatum("Type");
					m->setValueStr(EK_STR2WIDE("UserConfirmation"));
					m = feature->getMetadatum("Value");
					m->setValueStr(EK_STR2WIDE(atoms[0]->getMetadata("uCON",tag)));
				}
				atoms = atom->getAtoms("ffob.FEAD");
				if(atoms.size() > 0)
				{
					PmtCompositeMetadataPtr feature = (PmtCompositeMetadata *)face->modelGroup()->getMetadatum("Feature",++featureCount).ptr();
					m = feature->getMetadatum("Type");
					m->setValueStr(EK_STR2WIDE("OKAOData"));
					m = feature->getMetadatum("Data");
					m->setValueStr(EK_STR2WIDE(atoms[0]->getMetadata("FEAD",tag)));
				}
			}
		}
	}
	else if (container != NULL) // go from metadata to value
	{
		PmtCompositeMetadata * scenefaces = (PmtCompositeMetadata *)md;

		// For some reason, getMetadata won't return a composite metadata type.
		// Therefore, ask for something just below, and then back up.
		PmtMetadataIterator i = scenefaces->getMetadata("Object[*].Type",false);

		PmtContainerAtomPtr facesAtom;
		if (i.size() > 0)
		{
			std::vector<PmtAtom *> faces;
			PmtMetadataPtr ifaceType = i.start();
			while(ifaceType)
			{
				PmtCompositeMetadata * iface = (PmtCompositeMetadata *)ifaceType->parent();
				PmtContainerAtom * faceAtom = new PmtContainerAtom("ffob");

				PmtFaceFpdtAtom * fpdt = new PmtFaceFpdtAtom("fpdt");
				// Apparently, we're not supposed to use doubles,
				// only rationals, even though the spec says we
				// can!
				//fpdt->floatingPointRepresentation = 2;
				faceAtom->addAtom(fpdt);
                
				PmtMetadataPtr m = iface->getMetadatum("ID");
                if (m)
                {
                    PmtAtom * atom = PmtAtom::getFactory()->create("obID");
                    atom->setFromString(EK_WIDE2STR(m->getValueStr()));
					faceAtom->addAtom(atom);
                }
				m = iface->getMetadatum("Name");
                if (m)
                {
                    PmtAtom * atom = PmtAtom::getFactory()->create("obNa");
                    atom->setFromString(EK_WIDE2STR(m->getValueStr()));
					faceAtom->addAtom(atom);
                }
                
				PmtMetadataIterator j = iface->getMetadata("Feature[*].Type");
				PmtMetadataPtr ifeature = j.start();
				while (ifeature)
				{
					if (ifeature->getValueStr().length()>0)
					{
						SPmtString atomname;
						PmtCompositeMetadata * feature = (PmtCompositeMetadata *)ifeature->parent();
						SPmtString featureType = EK_WIDE2STR(ifeature->getValueStr());
						if (featureType == "Outline")
							atomname = "fout";
						else if (featureType == "LeftEye")
							atomname = "leye";
						else if (featureType == "RightEye")
							atomname = "reye";
						else if (featureType == "LeftOuterEyeCoordinate")
							atomname = "LOEC";
						else if (featureType == "LeftInnerEyeCoordinate")
							atomname = "LIEC";
						else if (featureType == "RightOuterEyeCoordinate")
							atomname = "ROEC";
						else if (featureType == "RightInnerEyeCoordinate")
							atomname = "RIEC";
						else if (featureType == "LeftMouthCenterPoint")
							atomname = "LMCP";
						else if (featureType == "RightMouthCenterPoint")
							atomname = "RMCP";
						else if (featureType == "FacePosePitchAngle")
							atomname = "FPPA";
						else if (featureType == "FacePoseYawAngle")
							atomname = "FPYA";
						else if (featureType == "FacePoseRotationAngle")
							atomname = "FPRA";
						else if (featureType == "FacePoseConfidenceLevel")
							atomname = "FPCL";
						else if (featureType == "UserConfirmation")
							atomname = "uCON";
						else if (featureType == "OKAOData")
							atomname = "FEAD";

						if (atomname.length() > 0)
						{
							PmtMetadataPtr mdpoints = feature->getMetadatum("Data",false);
							if (!mdpoints)
								mdpoints = feature->getMetadatum("Value",false);
							if (mdpoints)
							{
								EkString values = mdpoints->getValueStr();
								if (values.length()>0)
								{
									PmtAtom * atom = PmtAtom::getFactory()->create(atomname);
									atom->setFromString(EK_WIDE2STR(values));
									faceAtom->addAtom(atom);
								}
							}
						}
					}
					ifeature = j.next();
				}
				faces.push_back(faceAtom);
				ifaceType = i.next();
			}
			
			std::stringstream ostr(ios_base::in |ios_base::out | ios_base::binary);
			std::vector<PmtAtom *>::iterator iter = faces.begin();
			unsigned long long byteCount = 0;
			while (iter != faces.end())
			{
				byteCount += (*iter)->writeAtom(ostr);
				delete (*iter);
				iter++;
			}
			fvCount = (int) byteCount;
			vformatValue = new char[fvCount+1];
			memcpy(vformatValue,ostr.str().c_str(),fvCount);
		}
	}
}

