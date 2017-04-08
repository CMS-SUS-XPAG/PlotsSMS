from array import *

class sms():

    def __init__(self, modelname):
        if modelname.find("T1tttt") != -1: self.T1tttt()
        if modelname.find("T2tt") != -1: self.T2tt()
        if modelname.find("T5ttttDM175") != -1: self.T5ttttDM175()
        if modelname.find("T5tttt") != -1: self.T5tttt()
        if modelname.find("T1bbbb") != -1: self.T1bbbb()
        if modelname.find("T1qqqq") != -1: self.T1qqqq()
        if modelname.find("T5qqqqVV") != -1: self.T5qqqqVV()


    def T1tttt(self):
        # model name
        self.modelname = "T1tttt"
        # decay chain
        lsp_s = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}"
        self.label= "pp #rightarrow #tilde{g} #tilde{g}, #tilde{g} #rightarrow t #bar{t} "+lsp_s;
        self.label2= "";
        # scan range to plot
        self.Xmin = 600.
        self.Xmax = 2200.
        self.Ymin = 0.
        self.Ymax = 1900.
        self.Zmin = 0.001
        self.Zmax = 2.
        # produce sparticle
        self.sParticle = "m#kern[0.1]{_{#lower[-0.12]{#tilde{g}}}} [GeV]"
        # LSP
        self.LSP = "m#kern[0.1]{_{"+lsp_s+"}} [GeV]"
        # turn off diagonal lines
        self.diagOn = False
        self.boxOn = False
        
    def T2tt(self):
        # model name
        self.modelname = "T2tt"
        # decay chain
        lsp_s = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}"
        self.label= "pp #rightarrow #tilde{t} #tilde{t}, #tilde{t} #rightarrow t "+lsp_s;
        self.label2= "";
        # scan range to plot
        self.Xmin = 150.
        self.Xmax = 1200.
        self.Ymin = 0.
        self.Ymax = 800.
        self.Zmin = 0.001
        self.Zmax = 100.
        # produce sparticle
        self.sParticle = "m#kern[0.1]{_{#lower[-0.12]{#tilde{t}}}} [GeV]"
        # LSP
        self.LSP = "m#kern[0.1]{_{"+lsp_s+"}} [GeV]"
        # turn off diagonal lines
        self.diagOn = True
        self.boxOn = True
        
    def T5tttt(self):
        # model name
        self.modelname = "T1tttt"
        # decay chain
        lsp_s = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}"
        self.label= "pp #rightarrow #tilde{g} #tilde{g}, #tilde{g} #rightarrow t #bar{t} "+lsp_s;
        self.label2= "";
        # scan range to plot
        self.Xmin = 600.
        self.Xmax = 2200.
        self.Ymin = 0.
        self.Ymax = 1900.
        self.Zmin = 0.001
        self.Zmax = 2.
        # produce sparticle
        self.sParticle = "m#kern[0.1]{_{#lower[-0.12]{#tilde{g}}}} [GeV]"
        # LSP
        self.LSP = "m#kern[0.1]{_{"+lsp_s+"}} [GeV]"
        # turn off diagonal lines
        self.diagOn = False
        self.boxOn = False
        
    def T5ttttDM175(self):
        # model name
        self.modelname = "T5ttttDM175"
        # decay chain
        lsp_s = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}"
        self.label= "pp #rightarrow #tilde{g} #tilde{g},  #tilde{g} #rightarrow #tilde{t}_{1} t,  #tilde{t}_{1} #rightarrow #bar{t} "+lsp_s;
        self.label2= "m_{#tilde{t}_{1}} - m_{#tilde{#chi}^{0}_{1}} = 175 GeV";
        # scan range to plot
        self.Xmin = 600.
        self.Xmax = 1700.
        self.Ymin = 0.
        self.Ymax = 1800.
        self.Zmin = 0.001
        self.Zmax = 2.
        # produce sparticle
        self.sParticle = "m#kern[0.1]{_{#lower[-0.12]{#tilde{g}}}} [GeV]"
        # LSP
        self.LSP = "m#kern[0.1]{_{"+lsp_s+"}} [GeV]"
        # turn off diagonal lines
        self.diagOn = False
        self.boxOn = False
        
    def T1bbbb(self):
        # model name
        self.modelname = "T1bbbb"
        # decay chain
        lsp_s = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}"
        self.label= "pp #rightarrow #tilde{g} #tilde{g}, #tilde{g} #rightarrow b #bar{b} "+lsp_s;
        self.label2= "";
        # plot boundary. The top 1/4 of the y axis is taken by the legend
        self.Xmin = 600.
        self.Xmax = 2300.
        self.Ymin = 0.
        self.Ymax = 1900.
        self.Zmin = 0.001
        self.Zmax = 2.
        # produce sparticle
        self.sParticle = "m#kern[0.1]{_{#lower[-0.12]{#tilde{g}}}} [GeV]"
        # LSP
        self.LSP = "m#kern[0.1]{_{"+lsp_s+"}} [GeV]"
        # turn off diagonal lines
        self.diagOn = False
        self.boxOn = False

    def T1qqqq(self):
        # model name
        self.modelname = "T1qqqq"
        # decay chain
        lsp_s = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}"
        self.label= "pp #rightarrow #tilde{g} #tilde{g}, #tilde{g} #rightarrow q #bar{q} #tilde{#chi}^{0}_{1}";
        self.label2= "";
        # plot boundary. The top 1/4 of the y axis is taken by the legend
        self.Xmin = 600.
        self.Xmax = 2200.
        self.Ymin = 0.
        self.Ymax = 1900.
        self.Zmin = 0.001
        self.Zmax = 2.
        # produce sparticle
        self.sParticle = "m#kern[0.1]{_{#lower[-0.12]{#tilde{g}}}} [GeV]"
        # LSP
        self.LSP = "m#kern[0.1]{_{"+lsp_s+"}} [GeV]"
        # turn off diagonal lines
        self.diagOn = False
        self.boxOn = False

    def T5qqqqVV(self):
        # model name
        self.modelname = "T5qqqqVV"
        # decay chain
        lsp_s = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}"
        self.label= "pp #rightarrow #tilde{g} #tilde{g}, #tilde{g} #rightarrow q #bar{q} V #tilde{#chi}^{0}_{1}";
        self.label2= "";
        # plot boundary. The top 1/4 of the y axis is taken by the legend
        self.Xmin = 600.
        self.Xmax = 2200.
        self.Ymin = 0.
        self.Ymax = 1900.
        self.Zmin = 0.001
        self.Zmax = 2.
        # produce sparticle
        self.sParticle = "m#kern[0.1]{_{#lower[-0.12]{#tilde{g}}}} [GeV]"
        # LSP
        self.LSP = "m#kern[0.1]{_{"+lsp_s+"}} [GeV]"
        # turn off diagonal lines
        self.diagOn = False
        self.boxOn = False
