class Color :
    def __init__(self,form='hex'):
        self.blue=(57,106,177)
        self.orange=(218,124,48)
        self.green=(62,150,81)
        self.red=(204,37,41)
        self.black=(83,81,83)
        if form=='hex':
            self.blue=self.rgbtohex(self.blue)
            self.orange=self.rgbtohex(self.orange)
            self.green=self.rgbtohex(self.green)
            self.red=self.rgbtohex(self.red)
            self.black=self.rgbtohex(self.black)
        elif form == 'rgbnorm':
            self.blue=self.normalize_rgb(self.blue)
            self.orange=self.normalize_rgb(self.orange)
            self.green=self.normalize_rgb(self.green)
            self.red=self.normalize_rgb(self.red)
            self.black=self.normalize_rgb(self.black)
        elif form == 'rgbanorm':
            self.blue=self.normalize_rgba(self.blue)
            self.orange=self.normalize_rgba(self.orange)
            self.green=self.normalize_rgba(self.green)
            self.red=self.normalize_rgba(self.red)
            self.black=self.normalize_rgba(self.black)
    # retourne une couleur en hex au format #FF00FF à partir d'un tuple de valeurs RGB
    def rgbtohex(self,color):
        return '#'+''.join([hex(v)[-2:] for v in color])
    # retourne une liste de valeurs RGB normalisées entre 0 et 1 
    def normalize_rgb(self,color):
        return [c/255 for c in color]
    def normalize_rgba(self,color):
        print([c/255 for c in color]+[1.0])
        return [c/255 for c in color]+[1.0]
