#include <stdio.h>
#include "BDadosCoupe.h"

//Delegate functions for print list fields
void showTableFields(void *list) {
    ListaGenerica *temp = (ListaGenerica *) list;
    printf("A lista contem %d campos\n", temp->NEL);
    NOG *n = temp->Inicio;
    // NOG *n = (NOG *) list;
    printf("+----------------+--------------+\n");
    printf("| Field          | Type         |\n");
    printf("+----------------+--------------+\n");
    while (n) {
        CAMPO *c = (CAMPO *) n->Info;
        printf("| %s\t\t| %s\t\t|\n", c->NOME_CAMPO, c->TIPO);
        n = n->Prox;
    }
    printf("+----------------+--------------+\n");

}

void showTableData(void *list) {
    ListaGenerica *temp = (ListaGenerica *) list;
    printf("A lista contem %d registos\n", temp->NEL / 2);
    NOG *n = temp->Inicio;
    printf("+---------Conteudo Tabela--------+\n");
    //printf("+----------------+--------------+\n");
    printf("| ID             | Nome         |\n");
    int formatter = 1;
    while (n) {
        if (formatter > 2) {
            printf("|\n");
            formatter = 1;
        }
        printf("%s\t\t", n->Info);
        formatter++;
        n = n->Prox;
    }
    printf("|\n");
    printf("+----------------+--------------+\n");

}

int main() {
    BDadosCoupe *BD = Criar_BDados("BD-Banco", "Versao 1.0");

    TABELA *clientes = Criar_Tabela(BD, "CLIENTES");
    TABELA *moradas = Criar_Tabela(BD, "MORADAS");
    //Campos
    Add_Campo_Tabela(clientes, "ID", "INT");
    Add_Campo_Tabela(clientes, "NOME", "STRING");

    Add_Campo_Tabela(moradas, "ID", "CHAR");
    Add_Campo_Tabela(moradas, "LOCALIDADE", "STRING");

//Valores
    Add_Valores_Tabela(clientes, "1;Hugo");
    Add_Valores_Tabela(clientes, "2;Andre");
    Add_Valores_Tabela(clientes, "3;Paulo");

    Add_Valores_Tabela(moradas, "1;Viseu");
    Add_Valores_Tabela(moradas, "2;Lisboa");


    MostrarLG(clientes->LCampos, showTableFields);
    MostrarLG(clientes->LRegistos, showTableData);

    MostrarLG(moradas->LCampos, showTableFields);
    MostrarLG(moradas->LRegistos, showTableData);

    Mostrar_Tabela_NOME(BD, "MORADAS");
    Mostrar_BDados(BD);

    TABELA *t = Pesquisar_Tabela(BD, "CLIENTES");
    t != NULL ? printf("Tabela encontrada %s\n", t->NOME_TABELA) : printf("!!! Tabela nao encontrada !!!\n");


    Memoria_Desperdicada_BDados(BD);


    //Destruir_BDados(BD);
    return 0;
}
