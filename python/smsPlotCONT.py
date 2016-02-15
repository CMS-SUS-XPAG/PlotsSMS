import ROOT as rt
from array import *
from sms import *
from color import *
from smsPlotABS import *

# class producing the 2D plot with contours
class smsPlotCONT(smsPlotABS):

    def __init__(self, modelname, histo, obsLimits, expLimits, energy, lumi, preliminary, label):
        self.LABEL = label
        self.standardDef(modelname, histo, obsLimits, expLimits, energy, lumi, preliminary)
        # canvas for the plot
        self.c = rt.TCanvas("cCONT_%s" %label,"cCONT_%s" %label,600,600)
        self.histo = self.emptyHistogram(histo)
        # canvas style
        self.setStyle()

    # empty copy of the existing histogram
    def emptyHistogram(self, h):
        return rt.TH2D("%sEMPTY" %h['histogram'].GetName(), "%sEMPTY" %h['histogram'].GetTitle(),
                       h['histogram'].GetXaxis().GetNbins(), h['histogram'].GetXaxis().GetXmin(), h['histogram'].GetXaxis().GetXmax(),
                       h['histogram'].GetYaxis().GetNbins(), h['histogram'].GetYaxis().GetXmin(), h['histogram'].GetYaxis().GetXmax())
                                       
    def Draw(self):
        self.emptyHisto.Draw()
        self.histo.Draw("SAME")
        if self.model.diagOn:
            self.DrawDiagonal()
        self.DrawObsArea()
        self.DrawLines()
        self.DrawText()
        self.DrawLegend()

    def DrawObsArea(self):
        # add points to observed to close area
        # this will disappear
        self.OBS['nominal'].SetPoint(self.OBS['nominal'].GetN(), 1300,-1300)
        self.OBS['nominal'].SetPoint(self.OBS['nominal'].GetN(), -1300,-1300)
        # observed

        trasparentColor = rt.gROOT.GetColor(color(self.OBS['colorArea']))
        trasparentColor.SetAlpha(0.5)
        self.OBS['nominal'].SetFillColor(color(self.OBS['colorArea']))
        self.OBS['nominal'].SetLineStyle(1)
        # DRAW AREAS
        self.OBS['nominal'].Draw("FSAME")
