# Sistema de Atendimento Hospitalar — Squad ________

**Disciplina:** Programação e Sistemas Computacionais II (PSC II)
**Professor:** Giovane Galvão · **Instituição:** Campo Real — Engenharia de Software
**Integrantes:** _(preencher os nomes do squad)_

Triagem digital do Hospital Campo Real, implementada em C++ com **estruturas de dados lineares construídas manualmente** (sem STL), usando apenas a struct `No` fornecida.

## Estruturas e requisitos

| Req. | Estrutura | O que faz | Operações |
|------|-----------|-----------|-----------|
| R1 | **Fila** (FIFO, dois ponteiros `frente`/`tras`) | Pacientes aguardando atendimento | `enqueue`, `dequeue`, `frente_peek`, `tamanho` |
| R2 | **Pilha** (LIFO) | Histórico dos atendimentos concluídos | `push`, `pop`, `peek` |
| R3 | **Lista encadeada** | Médicos de plantão disponíveis; ao atender, o médico sai da lista e volta ao final | `adicionarMedico`, `chamarMedico`, `percorrer` |
| R4 | Relatório | Imprime o histórico do topo à base **sem destruí-lo**, usando uma pilha auxiliar | `imprimirHistorico` |

### Decisões de implementação
- **Fila:** quando `frente` vira `nullptr` no `dequeue`, `tras` também é zerado.
- **Pilha:** `push`, `pop` e `peek` operam sempre no topo, todas em O(1).
- **Lista:** `adicionarMedico` percorre até o último nó (inserção no final); `chamarMedico` remove do início.
- **Relatório:** os itens são movidos `historico → auxiliar` (imprimindo) e depois `auxiliar → historico`, restaurando a ordem original.
- **Memória:** todo `new` tem o `delete` correspondente (em `dequeue`, `pop`, `chamarMedico` e nos destrutores). Verificado com AddressSanitizer, sem vazamentos.

## Como compilar e executar

```bash
g++ -std=c++17 -Wall -o atendimento atendimento_template.cpp
./atendimento
```

Compila sem warnings. (Opcional, para checar vazamentos: `g++ -std=c++17 -Wall -fsanitize=address -g -o atendimento atendimento_template.cpp`.)

## Exemplo de saída

```text
╔═══════════════════════════════════════════════════╗
║  Sistema de Atendimento — Squad ________________  ║
╚═══════════════════════════════════════════════════╝

Médicos disponíveis: Dra. Ana Lima -> Dr. Bruno Costa -> Dra. Carla Ramos

Pacientes chegando:
Próximo a ser atendido: Eduardo (urgencia=3)
Total na fila: 4

Simulação de atendimentos:
  [1] Dra. Ana Lima atende Eduardo (urgencia=3)
  [2] Dr. Bruno Costa atende Fernanda (urgencia=1)
  [3] Dra. Carla Ramos atende Gabriel (urgencia=2)
  [4] Dra. Ana Lima atende Helena (urgencia=1)

Último atendimento registrado (peek): Dra. Ana Lima → Helena (urgencia=1)

=== Relatório de Atendimentos ===
  1. Dra. Ana Lima → Helena (urgencia=1)
  2. Dra. Carla Ramos → Gabriel (urgencia=2)
  3. Dr. Bruno Costa → Fernanda (urgencia=1)
  4. Dra. Ana Lima → Eduardo (urgencia=3)
Total de atendimentos: 4
```

## Estrutura do repositório

```text
squad-atendimento-[nome]/
├── atendimento_template.cpp   # código implementado
└── README.md                  # este arquivo
```
