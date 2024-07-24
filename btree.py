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
            varFilhos = varFilhos[0:grau]



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

    def printar(self):
        if not self.raiz:
            return

        queue = [(self.raiz, 0)]
        current_level = 0
        levels = {}

        while queue:
            node, level = queue.pop(0)
            if level not in levels:
                levels[level] = []
            levels[level].append(node.chaves)

            for filho in node.filhos:
                queue.append((filho, level + 1))

        print(f"Ordem da árvore: {self.grau}")
        print(f"Níveis: {len(levels)}")

        for level in sorted(levels.keys()):
            nodes_at_level = levels[level]
            formatted_nodes = ["-".join(map(str, node)) for node in nodes_at_level]
            print(f"Nível {level}: {', '.join(formatted_nodes)}")

BTree = BTree(3)
chaves = [10, 20, 5, 6, 12, 30, 7, 17]
for chave in chaves:
    BTree.insert(chave)

BTree.printar()