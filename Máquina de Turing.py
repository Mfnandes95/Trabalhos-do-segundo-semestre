class MaquinaTuring:
    def __init__(self, fita, estado_inicial="q0"):
        if not all(c in {'0', '1'} for c in fita if c != '□'):
            raise ValueError("A fita deve conter apenas '0', '1' ou '□'")
        
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
        self.max_passos = 1000  # Prevenção contra loops infinitos
        self.passo_a_passo = False

    def definir_modo_interativo(self, ativo):
        self.passo_a_passo = bool(ativo)

    def _validar_posicao(self):
        """Garante que o cabeçote está dentro dos limites da fita"""
        if self.cabecote < 0:
            self.cabecote = 0
        elif self.cabecote >= len(self.fita):
            self.fita.append('□')

    def executar(self):
        print(f"\n--- Executando Máquina de Turing ---")
        print(f"Fita inicial: {''.join(self.fita)}")
        
        passos = 0
        while passos < self.max_passos:
            passos += 1
            self._validar_posicao()
            
            simbolo = self.fita[self.cabecote]
            
            try:
                novo_estado, escreve, movimento = self.transicoes[self.estado][simbolo]
            except KeyError:
                print(f"\n⚠️ ERRO CRÍTICO: Transição indefinida para estado '{self.estado}' e símbolo '{simbolo}'")
                return False

            # Atualização segura da fita
            self.fita[self.cabecote] = escreve
            
            # Visualização
            marcador = " " * self.cabecote + "↓"
            print(f"\nPasso {passos}:")
            print(f"Estado: {self.estado} → {novo_estado}")
            print(f"Fita: {''.join(self.fita)}")
            print(f"       {marcador}")
            print(f"Ação: Escreve '{escreve}', Move {movimento}")

            if self.passo_a_passo:
                input("Pressione Enter para continuar...")

            # Atualização do estado e movimento
            self.estado = novo_estado
            
            if movimento == "R":
                self.cabecote += 1
            elif movimento == "L":
                self.cabecote -= 1
            elif movimento == "H":
                if novo_estado == "q_aceita":
                    print("\n✅ ACEITA: Número par de '1's detectado!")
                else:
                    print("\n❌ REJEITA: Número ímpar de '1's detectado!")
                return True

        print(f"\n⚠️ AVISO: Máquina parada após {self.max_passos} passos (possível loop infinito)")
        return False


def main():
    print("Simulador de Máquina de Turing - Verificador de Pares de '1's")
    
    while True:
        try:
            entrada = input("\nDigite uma string binária (ou 'sair'): ").strip()
            if entrada.lower() == 'sair':
                break
                
            if not entrada:
                print("⚠️ Por favor, digite uma entrada válida!")
                continue
                
            mt = MaquinaTuring(entrada)
            
            modo = input("Modo passo a passo? (s/n): ").lower()
            mt.definir_modo_interativo(modo.startswith('s'))
            
            mt.executar()
            
        except ValueError as e:
            print(f"Erro: {e}")
        except Exception as e:
            print(f"Erro inesperado: {e}")


if __name__ == "__main__":
    main()
