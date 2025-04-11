class MaquinaTuring:
    def __init__(self, fita, estado_inicial="q0"):
        self.fita = list(fita)  
        self.cabecote = 0       
        self.estado = estado_inicial
        self.transicoes = {
            "q0": {
                "0": ("q0", "0", "R"),
                "1": ("q_impar", "1", "R"),
                "□": ("q_aceita", "□", "H"),
            },
            "q_impar": {
                "0": ("q_impar", "0", "R"),
                "1": ("q0", "1", "R"),
                "□": ("q_rejeita", "□", "H"),
            }
        }
        self.passo_a_passo = False  # Modo interativo desativado por padrão

    def definir_modo_interativo(self, ativo):
        self.passo_a_passo = ativo

    def executar(self):
        print(f"\n--- Executando Máquina de Turing ---")
        print(f"Fita inicial: {''.join(self.fita)}")
        while True:
            simbolo = self.fita[self.cabecote] if 0 <= self.cabecote < len(self.fita) else "□"
            
            # Verifica se a transição é válida
            if self.estado not in self.transicoes or simbolo not in self.transicoes[self.estado]:
                print(f"ERRO: Não há transição definida para estado '{self.estado}' e símbolo '{simbolo}'!")
                return False
            
            novo_estado, escreve, movimento = self.transicoes[self.estado][simbolo]
            
            # Atualiza a fita
            if 0 <= self.cabecote < len(self.fita):
                self.fita[self.cabecote] = escreve
            else:
                self.fita.append(escreve) if movimento == "R" else self.fita.insert(0, escreve)
            
            # Exibe o passo atual
            marcador = " " * self.cabecote + "↓"
            print(f"\nEstado atual: {self.estado}")
            print(f"Fita: {''.join(self.fita)}")
            print(f"Posição: {marcador}")
            print(f"Próximo: {novo_estado}, Escreve: '{escreve}', Move: {movimento}")
            
            if self.passo_a_passo:
                input("Pressione Enter para continuar...")
            
            self.estado = novo_estado
            
            # Move o cabeçote
            if movimento == "R":
                self.cabecote += 1
            elif movimento == "L":
                self.cabecote -= 1
                if self.cabecote < 0:  # Evita sair da fita pela esquerda
                    self.cabecote = 0
            elif movimento == "H":
                if novo_estado == "q_aceita":
                    print("\n✅ ACEITA: A entrada foi reconhecida!")
                else:
                    print("\n❌ REJEITA: A entrada NÃO foi reconhecida!")
                return novo_estado == "q_aceita"

# Testando a máquina
if __name__ == "__main__":
    entrada = input("Digite uma string binária (ex: '1010'): ").strip() or "1010"
    mt = MaquinaTuring(entrada)
    
    modo_interativo = input("Executar passo a passo? (s/n): ").lower().startswith("s")
    mt.definir_modo_interativo(modo_interativo)
    
    mt.executar()101
