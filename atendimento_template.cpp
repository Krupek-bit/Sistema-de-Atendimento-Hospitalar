// ╔══════════════════════════════════════════════════════════════════════════╗
// ║  DESAFIO SQUAD — Sistema de Atendimento                                 ║
// ║  PSC II · Engenharia de Software · Campo Real · Prof. Giovane Galvão    ║
// ╠══════════════════════════════════════════════════════════════════════════╣
// ║  Compilar:  g++ -std=c++17 -Wall -o atendimento atendimento_template.cpp ║
// ║  Rodar:     ./atendimento                                                ║
// ╠══════════════════════════════════════════════════════════════════════════╣
// ║  REGRAS:                                                                 ║
// ║   • Não usar STL (vector, queue, stack, list etc.)                       ║
// ║   • Toda alocação dinâmica (new) deve ter seu delete correspondente      ║
// ║   • Commitar a cada requisito implementado (R1, R2, R3, R4)             ║
// ║   • README.md no repositório explicando como compilar e usar             ║
// ╠══════════════════════════════════════════════════════════════════════════╣
// ║  RANKING:                                                                ║
// ║   +1 por requisito que compila e funciona                                ║
// ║   +1 por commit organizado com mensagem descritiva                       ║
// ║   +2 por README.md completo                                              ║
// ║   +2 squad com melhor demo (2 min)                                       ║
// ╚══════════════════════════════════════════════════════════════════════════╝

#include <iostream>
#include <string>
using namespace std;

// ════════════════════════════════════════════════════════════════
// ESTRUTURA BASE — Nó genérico (NÃO MODIFIQUE)
// ════════════════════════════════════════════════════════════════
struct No {
    string nome;
    int    urgencia;   // 1=baixa, 2=media, 3=alta (usado apenas por Paciente)
    No*    prox;
    No(const string& n, int u = 0) : nome(n), urgencia(u), prox(nullptr) {}
};

// ════════════════════════════════════════════════════════════════
// R1 · FILA DE PACIENTES (estrutura FIFO com dois ponteiros)
// ════════════════════════════════════════════════════════════════
// Pacientes entram pelo "trás" e saem pela "frente"
// Dentro da mesma urgência, quem chegou primeiro é atendido primeiro

struct FilaPacientes {
    No* frente = nullptr;
    No* tras   = nullptr;

    // TODO 1: enqueue(nome, urgencia) — paciente entra no final da fila
    void enqueue(const string& nome, int urgencia) {
        No* novo = new No(nome, urgencia);
        if (isEmpty()) {          // fila vazia: o novo nó é frente e trás
            frente = novo;
            tras   = novo;
        } else {                  // senão, liga no final e atualiza 'tras'
            tras->prox = novo;
            tras       = novo;
        }
    }

    // TODO 2: dequeue() — remove e retorna o nome do paciente da frente
    string dequeue() {
        if (isEmpty()) return "";
        No* remover   = frente;
        string nome   = remover->nome;   // guarda o nome antes de deletar
        frente        = frente->prox;
        if (frente == nullptr)           // fila ficou vazia: zera 'tras' também
            tras = nullptr;
        delete remover;
        return nome;
    }

    // TODO 3: frente_peek() — ver quem está na frente sem remover
    string frente_peek() const {
        if (isEmpty()) return "";
        return frente->nome;
    }

    bool isEmpty() const { return frente == nullptr; }

    // BÔNUS: tamanho() conta os nós na fila (iterativo)
    int tamanho() const {
        int cont = 0;
        for (No* p = frente; p != nullptr; p = p->prox) cont++;
        return cont;
    }

    ~FilaPacientes() {
        while (!isEmpty()) dequeue();
    }
};

// ════════════════════════════════════════════════════════════════
// R2 · HISTÓRICO COM PILHA (estrutura LIFO)
// ════════════════════════════════════════════════════════════════
// Cada atendimento concluído vai para a pilha de histórico
// O médico pode consultar o último atendimento com peek()

struct PilhaHistorico {
    No* topo = nullptr;

    // TODO 4: push(nome) — empilha um atendimento concluído
    void push(const string& nome) {
        No* novo  = new No(nome);
        novo->prox = topo;     // novo nó aponta para o antigo topo
        topo       = novo;     // e passa a ser o topo
    }

    // TODO 5: pop() — desempilha e retorna o último atendimento
    string pop() {
        if (isEmpty()) return "";
        No* remover = topo;
        string nome = remover->nome;
        topo        = topo->prox;
        delete remover;
        return nome;
    }

    // TODO 6: peek() — retorna o último atendimento SEM remover
    string peek() const {
        if (isEmpty()) return "";
        return topo->nome;
    }

    bool isEmpty() const { return topo == nullptr; }

    ~PilhaHistorico() {
        while (!isEmpty()) pop();
    }
};

// ════════════════════════════════════════════════════════════════
// R3 · LISTA DE MÉDICOS DISPONÍVEIS (lista encadeada)
// ════════════════════════════════════════════════════════════════
// Lista de médicos plantão. Quando chamado, médico sai da lista.
// Quando termina o atendimento, volta para o final da lista.

struct ListaMedicos {
    No* head = nullptr;

    // TODO 7: adicionarMedico(nome) — insere no final da lista
    void adicionarMedico(const string& nome) {
        No* novo = new No(nome);
        if (head == nullptr) {    // lista vazia: o novo nó vira o head
            head = novo;
            return;
        }
        No* p = head;             // percorre até o último nó
        while (p->prox != nullptr) p = p->prox;
        p->prox = novo;
    }

    // TODO 8: chamarMedico() — remove e retorna o primeiro médico disponível
    string chamarMedico() {
        if (head == nullptr) return "";
        No* remover = head;
        string nome = remover->nome;
        head        = head->prox;
        delete remover;
        return nome;
    }

    // TODO 9: percorrer() — imprime todos os médicos disponíveis
    void percorrer(const string& prefixo = "") const {
        if (!prefixo.empty()) cout << prefixo << ": ";
        if (head == nullptr) {
            cout << "(lista vazia)\n";
            return;
        }
        for (No* p = head; p != nullptr; p = p->prox) {
            cout << p->nome;
            if (p->prox != nullptr) cout << " -> ";
        }
        cout << "\n";
    }

    bool temMedico() const { return head != nullptr; }

    ~ListaMedicos() {
        while (head) {
            No* t = head;
            head  = head->prox;
            delete t;
        }
    }
};

// ════════════════════════════════════════════════════════════════
// R4 · RELATÓRIO E HISTÓRICO COMPLETO
// ════════════════════════════════════════════════════════════════

// TODO 10: imprimirHistorico — imprime do topo à base SEM destruir o histórico.
// Estratégia: desempilha tudo para uma pilha auxiliar (imprimindo),
// depois devolve da auxiliar para a original, restaurando a ordem.
void imprimirHistorico(PilhaHistorico& hist) {
    cout << "\n=== Relatório de Atendimentos ===\n";
    if (hist.isEmpty()) {
        cout << "(nenhum atendimento registrado)\n";
        return;
    }

    PilhaHistorico aux;
    int n = 1;

    // 1) hist -> aux, imprimindo (do topo à base)
    while (!hist.isEmpty()) {
        string item = hist.pop();
        cout << "  " << n++ << ". " << item << "\n";
        aux.push(item);
    }

    // 2) aux -> hist, restaurando a ordem original
    while (!aux.isEmpty()) {
        hist.push(aux.pop());
    }

    cout << "Total de atendimentos: " << (n - 1) << "\n";
}

// ════════════════════════════════════════════════════════════════
// PROGRAMA PRINCIPAL — integra as 4 estruturas
// ════════════════════════════════════════════════════════════════

int main() {
    cout << "╔═══════════════════════════════════════════════════╗\n";
    cout << "║  Sistema de Atendimento — Squad ________________  ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n\n";

    // Crie as estruturas
    FilaPacientes  fila;
    PilhaHistorico historico;
    ListaMedicos   medicos;

    // ── Cadastrar médicos (R3) ────────────────────────────────
    medicos.adicionarMedico("Dra. Ana Lima");
    medicos.adicionarMedico("Dr. Bruno Costa");
    medicos.adicionarMedico("Dra. Carla Ramos");
    medicos.percorrer("Médicos disponíveis");

    // ── Pacientes chegam na fila (R1) ─────────────────────────
    cout << "\nPacientes chegando:\n";
    fila.enqueue("Eduardo (urgencia=3)", 3);
    fila.enqueue("Fernanda (urgencia=1)", 1);
    fila.enqueue("Gabriel (urgencia=2)", 2);
    fila.enqueue("Helena (urgencia=1)", 1);

    cout << "Próximo a ser atendido: " << fila.frente_peek() << "\n";
    cout << "Total na fila: " << fila.tamanho() << "\n";

    // ── Simulação de atendimentos ─────────────────────────────
    cout << "\nSimulação de atendimentos:\n";
    int atend = 1;
    while (!fila.isEmpty() && medicos.temMedico()) {
        string paciente = fila.dequeue();
        string medico   = medicos.chamarMedico();

        cout << "  [" << atend++ << "] " << medico
             << " atende " << paciente << "\n";

        // Registra no histórico (R2)
        historico.push(medico + " → " + paciente);

        // Médico volta para a fila depois de atender
        medicos.adicionarMedico(medico);
    }

    // ── Último atendimento (peek na pilha) ────────────────────
    cout << "\nÚltimo atendimento registrado (peek): " << historico.peek() << "\n";

    // ── Relatório final (R4) ──────────────────────────────────
    imprimirHistorico(historico);

    // ── Commit sugerido ──────────────────────────────────────
    cout << "\n💡 Lembrete de commits:\n";
    cout << "  git add atendimento_template.cpp README.md\n";
    cout << "  git commit -m \"feat: sistema de atendimento integrado (R1-R4)\"\n";
    cout << "  git push origin main\n";

    return 0;
}

// ════════════════════════════════════════════════════════════════
// CHECKLIST DE ENTREGA
// ════════════════════════════════════════════════════════════════
// [ ] TODO 1 — enqueue funciona (paciente entra na fila)
// [ ] TODO 2 — dequeue funciona (paciente sai da fila, memória liberada)
// [ ] TODO 3 — frente_peek retorna o nome sem remover
// [ ] TODO 4 — push empilha no histórico
// [ ] TODO 5 — pop desempilha e libera memória
// [ ] TODO 6 — peek retorna o topo sem remover
// [ ] TODO 7 — adicionarMedico insere no final da lista
// [ ] TODO 8 — chamarMedico remove do início e retorna o nome
// [ ] TODO 9 — percorrer imprime todos os médicos com setas
// [ ] TODO 10 — imprimirHistorico imprime sem destruir os dados
// [ ] Programa compila sem warnings: g++ -std=c++17 -Wall -o sis main.cpp
// [ ] Sem vazamentos de memória (todo new tem delete correspondente)
// [ ] Repositório no GitHub com histórico de commits organizado
// [ ] README.md com instrução de compilação e exemplo de saída
