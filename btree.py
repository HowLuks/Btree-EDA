class BTreeNo:
    def __init__(self, grau, folha=False):
        self.grau = grau  
        self.folha = folha  
        self.chaves = [] 
        self.filhos = [] 

    def inserir(self, chave):
        aux = len(self.chaves) - 1
        if self.folha:
            self.chaves.append(None)
            while aux >= 0 and self.chaves[aux] > chave:
                self.chaves[aux+1] = self.chaves[aux]
                aux -= 1
            self.chaves[aux+1] = chave
        else:
            while aux >= 0 and self.chaves[aux] > chave:
                aux -= 1
            aux += 1
            
            if len(self.filhos[aux].chaves) == 2 * self.grau - 1:
                self.split(aux)
                if self.chaves[aux] < chave:
                    aux += 1
            self.filhos[aux].inserir(chave)

    def split(self, aux):
        grau = self.grau
        varFilhos = self.filhos[aux]
        noAux = BTreeNo(grau, varFilhos.folha)
        self.filhos.insert(aux+1, noAux)
        self.chaves.insert(aux, varFilhos.chaves[grau-1])

        noAux.chaves = varFilhos.chaves[grau:(2 * grau - 1)]
        varFilhos.chaves = varFilhos.chaves[0:(grau - 1)]

        if not varFilhos.folha:
            noAux.filhos = varFilhos.filhos[grau:(2*grau)]
            varFilhos = varFilhos.filhos[0:grau]



class BTree:
    def __init__(self, grau):
        self.raiz = BTreeNo(grau, True)
        self.grau = grau
        
    def insert(self, chave):
        raiz = self.raiz
        if len(raiz.chaves) == 2 * self.grau - 1:
            raizAux = BTreeNo(self.grau, folha=False)
            raizAux.filhos.append(self.raiz)
            raizAux.split(0)
            raizAux.inserir(chave)
            self.raiz = raizAux
        else:
            raiz.inserir(chave)


def ler_arquivo(nome):
    with open(nome, "r") as entrada:
        dados = [int(line.strip()) for line in entrada]

        return dados
        
def escrever_arquivo(nome, btree):
    with open(nome, "w") as saida:
        if not btree.raiz:
            return

        queue = [(btree.raiz, 0)]
        niveis = {}

        while queue:
            no, nivel = queue.pop(0)
            if nivel not in niveis:
                niveis[nivel] = []
            niveis[nivel].append(no.chaves)

            for filho in no.filhos:
                queue.append((filho, nivel + 1))

        saida.write(f"grau da arvore b:{btree.grau} |  Níveis: {len(niveis)}\n")
        for nivel in sorted(niveis.keys()):
            nospornivel = niveis[nivel]
            nosformatados = [", ".join(map(str, no)) for no in nospornivel]
            saida.write(f"Nível {nivel}: {' - '.join(nosformatados)}\n")
    
    
dados = ler_arquivo("entrada 2.txt")
for num in range(0, len(dados)):
            if num == 0:
                grau = int(dados[0])
                BTree = BTree(grau)
            else:
                BTree.insert(int(dados[num]))

escrever_arquivo("saida.txt", BTree)