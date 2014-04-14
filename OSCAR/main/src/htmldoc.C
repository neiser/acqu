// SVN Info: $Id: htmldoc.C 1202 2012-06-06 11:58:07Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2007
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
    if (!gROOT->GetClass("TOGlobals")) gSystem->Load("libOSCAR.so");

    gEnv->SetValue("Unix.*.Root.Html.SourceDir", "A2/src:A2/include:analysis/src:analysis/include:"
                                                 "graph/src:graph/include:main/src:main/include:"
                                                 "MC/src:MC/include:utils/src:utils/include:"
                                                 "math/src:math/include");
    THtml h;
    h.SetOutputDir("htmldoc");
    h.SetAuthorTag(" * Author:");
    h.SetProductName("OSCAR");

    h.MakeAll();
}

