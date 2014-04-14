// SVN Info: $Id: htmldoc.C 1022 2011-10-10 14:51:59Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// htmldoc.C                                                            //
//                                                                      //
// ROOT macro for the creation of the HTML documentation.               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void htmldoc()
{
    gROOT->Reset();
    if (!gROOT->GetClass("TCCalib")) gSystem->Load("libCaLib.so");

    THtml h;
    h.SetInputDir(".:src:include");
    h.SetOutputDir("htmldoc");
    h.SetAuthorTag("* Author:");
    h.SetProductName("CaLib");

    h.MakeAll();
}

