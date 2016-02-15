import ROOT as rt
from array import *
from sms import *
from color import *
from smsPlotABS import *

# class producing the 2D plot with contours
class smsPlotBrazil(smsPlotABS):

    def __init__(self, modelname, histo, obsLimits, expLimits, energy, lumi, preliminary, label):
        self.LABEL = label
        self.standardDef(modelname, histo, obsLimits, expLimits, energy, lumi, preliminary)
        # canvas for the plot
        self.c = rt.TCanvas("cBrazil_%s" %label,"cBrazil_%s" %label,600,600)
        self.histo = self.emptyHistogram(histo)
        # canvas style
        self.setStyle()

    # empty copy of the existing histogram
    def emptyHistogram(self, h):
        hname = h['histogram'].GetName()+"EMPTY_"+self.LABEL
        return rt.TH2D(hname, "%sEMPTY" %h['histogram'].GetTitle(),
                       h['histogram'].GetXaxis().GetNbins(), h['histogram'].GetXaxis().GetXmin(), h['histogram'].GetXaxis().GetXmax(),
                       h['histogram'].GetYaxis().GetNbins(), h['histogram'].GetYaxis().GetXmin(), h['histogram'].GetYaxis().GetXmax())
                                       
    def Draw(self):
        self.emptyHisto.GetXaxis().SetRangeUser(self.model.Xmin, self.model.Xmax)
        self.emptyHisto.GetYaxis().SetRangeUser(self.model.Ymin, self.model.Ymax)
        self.emptyHisto.Draw()
        self.histo.Draw("SAME")
        if self.model.diagOn:
            self.DrawDiagonal()
        self.DrawLines()
        self.DrawText()
        self.DrawLegend()

    def DrawLines(self):
        # observed
        self.OBS['nominal'].SetLineColor(1)
        self.OBS['nominal'].SetLineStyle(1)
        self.OBS['nominal'].SetLineWidth(4)
        # observed + 1sigma
        self.OBS['plus'].SetLineColor(1)
        self.OBS['plus'].SetLineWidth(2)
        self.OBS['plus'].SetLineStyle(1)
        # observed - 1sigma
        self.OBS['minus'].SetLineColor(1)
        self.OBS['minus'].SetLineWidth(2)
        self.OBS['minus'].SetLineStyle(1)
        # expected
        self.EXP['nominal'].SetLineColor(1)
        self.EXP['nominal'].SetLineWidth(4)
        self.EXP['nominal'].SetLineStyle(2)        
        # build one graph summing the + and - 1 sigma
        nP = self.EXP['plus'].GetN()
        nM = self.EXP['minus'].GetN()
        sigmaBandX = []
        sigmaBandY = []
        for i in range(0,nP):
            sigmaBandX.append(rt.Double(0.))
            sigmaBandY.append(rt.Double(0.))
            self.EXP['plus'].GetPoint(i, sigmaBandX[i], sigmaBandY[i])          
        for i in range(0,nM):
            sigmaBandX.append(rt.Double(0.))
            sigmaBandY.append(rt.Double(0.))
            #self.EXP['minus'].GetPoint(i, sigmaBandX[i+nP], sigmaBandY[i+nP])                      
        for i in range(0,nM):            
            self.EXP['minus'].GetPoint(i, sigmaBandX[nP+nM-i-1], sigmaBandY[nP+nM-i-1])          
        sigmaBand = rt.TGraph(nP+nM, array('d', sigmaBandX), array('d', sigmaBandY))
        #sigmaBand.SetFillStyle(3001)
        sigmaBand.SetFillColor(color(self.EXP['colorArea']))
        sigmaBand.Draw("FSAME")
        self.c.sigmaBand = sigmaBand
        
        # expected + 1sigma
        self.EXP['plus'].SetLineColor(1)
        self.EXP['plus'].SetLineStyle(3)
        # expected - 1sigma
        self.EXP['minus'].SetLineColor(1)
        self.EXP['minus'].SetLineStyle(3)
        # DRAW LINES
        self.OBS['nominal'].Draw("LSAME")
        self.OBS['plus'].Draw("LSAME")
        self.OBS['minus'].Draw("LSAME")        
        self.EXP['nominal'].Draw("LSAME")
        #self.EXP['plus'].Draw("LSAME")
        #self.EXP['minus'].Draw("LSAME")        

    def DrawLegend(self):
        xRange = self.model.Xmax-self.model.Xmin
        yRange = self.model.Ymax-self.model.Ymin

        LObs = rt.TGraph(2)
        LObs.SetName("LObs")
        LObs.SetTitle("LObs")
        LObs.SetLineColor(1)
        LObs.SetLineStyle(1)
        LObs.SetLineWidth(4)
        LObs.SetMarkerStyle(20)
        LObs.SetPoint(0,self.model.Xmin+3*xRange/100, self.model.Ymax-1.35*yRange/100*10)
        LObs.SetPoint(1,self.model.Xmin+10*xRange/100, self.model.Ymax-1.35*yRange/100*10)

        LObsP = rt.TGraph(2)
        LObsP.SetName("LObsP")
        LObsP.SetTitle("LObsP")
        LObsP.SetLineColor(1)
        LObsP.SetLineStyle(1)
        LObsP.SetLineWidth(2)
        LObsP.SetMarkerStyle(20)
        LObsP.SetPoint(0,self.model.Xmin+3*xRange/100, self.model.Ymax-1.20*yRange/100*10)
        LObsP.SetPoint(1,self.model.Xmin+10*xRange/100, self.model.Ymax-1.20*yRange/100*10)

        LObsM = rt.TGraph(2)
        LObsM.SetName("LObsM")
        LObsM.SetTitle("LObsM")
        LObsM.SetLineColor(1)
        LObsM.SetLineStyle(1)
        LObsM.SetLineWidth(2)
        LObsM.SetMarkerStyle(20)
        LObsM.SetPoint(0,self.model.Xmin+3*xRange/100, self.model.Ymax-1.50*yRange/100*10)
        LObsM.SetPoint(1,self.model.Xmin+10*xRange/100, self.model.Ymax-1.50*yRange/100*10)

        textObs = rt.TLatex(self.model.Xmin+11*xRange/100, self.model.Ymax-1.50*yRange/100*10, "Observed #pm 1 #sigma_{theory}")
        textObs.SetTextFont(42)
        textObs.SetTextSize(0.040)
        textObs.Draw()
        self.c.textObs = textObs

        LExp = rt.TGraph(2)
        LExp.SetName("LExp")
        LExp.SetTitle("LExp")
        LExp.SetLineColor(1)
        LExp.SetLineWidth(4)
        LExp.SetLineStyle(2)
        LExp.SetPoint(0,self.model.Xmin+3*xRange/100, self.model.Ymax-2.00*yRange/100*10)
        LExp.SetPoint(1,self.model.Xmin+10*xRange/100, self.model.Ymax-2.00*yRange/100*10)

        LExpP = rt.TGraph(4)
        LExpP.SetName("LExpPM")
        LExpP.SetTitle("LExpPM")
        LExpP.SetFillColor(color(self.EXP['colorArea']))
        LExpP.SetLineStyle(3)
        LExpP.SetPoint(0,self.model.Xmin+10,43)
        LExpP.SetPoint(1,self.model.Xmin+80,43)
        LExpP.SetPoint(2,self.model.Xmin+80,27)
        LExpP.SetPoint(3,self.model.Xmin+10,27)
        
        LExpP.SetPoint(0,self.model.Xmin+3*xRange/100, self.model.Ymax-2.15*yRange/100*10)
        LExpP.SetPoint(1,self.model.Xmin+10*xRange/100, self.model.Ymax-2.15*yRange/100*10)
        LExpP.SetPoint(2,self.model.Xmin+10*xRange/100, self.model.Ymax-1.85*yRange/100*10)
        LExpP.SetPoint(3,self.model.Xmin+3*xRange/100, self.model.Ymax-1.85*yRange/100*10)

        
        textExp = rt.TLatex(self.model.Xmin+11*xRange/100, self.model.Ymax-2.15*yRange/100*10, "Expected #pm 1 #sigma_{experiment}")
        textExp.SetTextFont(42)
        textExp.SetTextSize(0.040)
        textExp.Draw()
        self.c.textExp = textExp

        LObs.Draw("LSAME")
        LObsM.Draw("LSAME")
        LObsP.Draw("LSAME")
        LExpP.Draw("FSAME")
        LExp.Draw("LSAME")
        
        self.c.LObs = LObs
        self.c.LObsM = LObsM
        self.c.LObsP = LObsP
        self.c.LExp = LExp
        self.c.LExpP = LExpP
