
#include <stdint.h>
#include "BDadosCoupe.h"

#define FIXED_SIZE_ARRAY 50

/** \brief Criar_BDados: A) Criar a Base de dados
 *
 * \param nome_bd char* : Nome da Base de Dados
 * \param versao char*  : Versão da Base de Dados
 * \return BDadosCoupe* : Ponteiro para a Base de Dados Criada
 *
 */
BDadosCoupe *Criar_BDados(char *nome_bd, char *versao) {
    BDadosCoupe *BD = (BDadosCoupe *) malloc(sizeof(BDadosCoupe));
    strcpy(BD->NOME_BDADOS, nome_bd);
    strcpy(BD->VERSAO_BDADOS, versao);
    BD->LTabelas = CriarLG();
    return BD;
}

/** \brief B) Criar uma Tabela na Base de Dados,
 * a tabela deve ser inserida à lista de tabelas da BD
 *
 * \param BD BDadosCoupe* : Ponteiro para a Base de Dados
 * \param nome_tabela char* : Nome da tabela a Criar
 * \return TABELA*        : Retorna a Tabela Criada
 * \author                : CS, FM & JL
 */
TABELA *Criar_Tabela(BDadosCoupe *BD, char *nome_tabela) {
    TABELA *table = (TABELA *) malloc(sizeof(TABELA));

    strcpy(table->NOME_TABELA, nome_tabela);
    table->LCampos = CriarLG();
    table->LRegistos = CriarLG();

    AddLG(BD->LTabelas, table);
    return table;
}

/** \brief C)  Adicionar um campo a uma tabela.
 *
 * \param T TABELA* : Ponteiro para a tabela
 * \param nome_campo char* : Nome do campo
 * \param tipo_campo char* : Tipo do Campo (INT, STRING, CHAR, FLOAT)
 * \return int             : SUCESSO/INSUCESSO
 * \author                 : CS, FM & JL              :
 * \version                : Versao 1.0
 * \date                   : 23/04/2022
 */
int Add_Campo_Tabela(TABELA *T, char *nome_campo, char *tipo_campo) {
    if (!T) return INSUCESSO;
    CAMPO *campo = (CAMPO *) malloc(sizeof(CAMPO));
    strcpy(campo->NOME_CAMPO, nome_campo);
    strcpy(campo->TIPO, tipo_campo);
    return AddLG(T->LCampos, campo);
}

//D)	Adicionar dados(registos) a uma tabela, os dados são dados numa string onde o separador é ‘;’m ex: Add_Valores_Tabela(T, “123;Joao;965654449”)
int Add_Valores_Tabela(TABELA *T, char *dados) {
    if (dados) {
        char *tmp = (char *) malloc(sizeof(strlen(dados) + 1));
        strcpy(tmp, dados);
        char *data = strtok(tmp, ";");
        while (data != NULL) {
            char *registo = (char *) malloc(sizeof(strlen(data) + 1));
            strcpy(registo, data);
            data = strtok(NULL, ";");
            AddLG(T->LRegistos, registo);
        }
        free(tmp);
        return SUCESSO;
    }
    return INSUCESSO;
}


/**
 * @param BD : Database name
 * @param nome_tabela :Name of table
 * @param dados :Data to be inserted in the table.\n
 * Note: Data must be a string with with values separated by character ;
 * @return : SUCESSO - Case of success\n
 *          :INSUCESSO - Case of fail
 */
int Add_Valores_Tabela_BDados(BDadosCoupe *BD, char *nome_tabela, char *dados) {
    TABELA *T = Pesquisar_Tabela(BD, nome_tabela);
    if (T) {
        return Add_Valores_Tabela(T, dados);
    }
    return INSUCESSO;
}

//F)	Pesquisar uma Tabela da base de dados
TABELA *Pesquisar_Tabela(BDadosCoupe *BD, char *nome_tabela) {
    if (!BD)return NULL;
    if (strlen(nome_tabela) < 1) {
        printf("Nome da tabela invalido.Indique o nome da tabela!\n");
        return NULL;
    }
    //Iterate the list
    if (BD->LTabelas->NEL) {
        NOG *n = BD->LTabelas->Inicio;
        while (n) {
            TABELA *t = (TABELA *) n->Info;
            if (strcmp(nome_tabela, t->NOME_TABELA) == 0)return t;
            n = n->Prox;
        }
        return NULL;
    }
    return NULL;
}

//G)	Mostrar uma Tabela
void Mostrar_Tabela_NOME(BDadosCoupe *BD, char *tabela) {
    if (!BD)return;
    if (strlen(tabela) < 1) {
        printf("Nome da tabela invalido.Indique o nome da tabela!\n");
        return;
    }
    TABELA *t = Pesquisar_Tabela(BD, tabela);
    if (t != NULL) {
        Mostrar_Tabela(t);
    }
}

void Mostrar_Tabela(TABELA *T) {
    printf("Mostrando a tabela %s\n", T->NOME_TABELA);
    printf("Desenhar no futuro ...\n");
}

//H)	Mostrar toda a base de dados, deverá mostrar todas as Tabelas da BDados.
void Mostrar_BDados(BDadosCoupe *BD) {
    if (!BD)return;
    printf("--------Tabelas disponiveis na BD-----------\n");
    ListaGenerica *head = (ListaGenerica *) BD->LTabelas;
    NOG *node = head->Inicio;

    while (node) {
        TABELA *t = (TABELA *) node->Info;
        Mostrar_Tabela(t);
        printf("--------TABELA -----------\n");
        node = node->Prox;
    }

}

//I)	Libertar toda a memória alocada pela base de dados.
void Destruir_BDados(BDadosCoupe *BD) {
    if (!BD)return;
    if (BD->LTabelas->NEL > 0) {
        NOG *n = BD->LTabelas->Inicio;
        printf("%d\n", BD->LTabelas->NEL);
        while (n) {
            TABELA *t = n->Info;
            printf("%s\n", t->NOME_TABELA);
            DELETE_TABLE_DATA(t);
            printf("%d\n", BD->LTabelas->NEL);
            n = n->Prox;
        }
    }
    free(BD->LTabelas);
    printf("Destruindo BD");
    free(BD);

}

//J)	Memória ocupada por toda a base de dados.
long int Memoria_BDados(BDadosCoupe *BD) {
    return -1;
}

long int Memoria_Desperdicada_BDados(BDadosCoupe *BD) {
    if (!BD) return -1;
    long int wasted_memory = 0;
    //Evaluate the memory used in BD head
    wasted_memory += FIXED_SIZE_ARRAY - (strlen(BD->NOME_BDADOS) + 1);
    wasted_memory += FIXED_SIZE_ARRAY - (strlen(BD->VERSAO_BDADOS) + 1);
    //Evaluate the memory in each ListTabelas - nome_tabela
    ListaGenerica *head = (ListaGenerica *) BD->LTabelas;
    //printf("A lista contem %d campos\n", head->NEL);
    NOG *node = head->Inicio;
    while (node) {
        TABELA *t = (TABELA *) node->Info;
        wasted_memory += FIXED_SIZE_ARRAY - (strlen(t->NOME_TABELA) + 1);
        NOG *node_LCampos = t->LCampos->Inicio;
        while (node_LCampos) {
            //printf("looping node campo\n");
            CAMPO *c = (CAMPO *) node_LCampos->Info;
            wasted_memory += FIXED_SIZE_ARRAY - (strlen(c->TIPO) + 1);
            //printf("Memoria do tipo da campo %s %d\t\t\n", c->TIPO, strlen(c->TIPO) + 1);
            wasted_memory += FIXED_SIZE_ARRAY - (strlen(c->NOME_CAMPO) + 1);
            //printf("Memoria do nome da campo %s %d\t\t\n", c->NOME_CAMPO, strlen(c->NOME_CAMPO) + 1);
            node_LCampos = node_LCampos->Prox;
        }
        node = node->Prox;
    }

    printf("Memoria desperdicada na BD %d BYTES \n", wasted_memory);
    return wasted_memory;

    //Analisar desperdicio nos seguintes campos
//    BD
//      char NOME_BDADOS[50];
//      char VERSAO_BDADOS[50];
//  TABELA
//    char NOME_TABELA[50];
//  CAMPO
//    char NOME_CAMPO[50];
//    char TIPO[10];
//  REGISTO


}

//K)	Exportar/Importar para/de Ficheiro (o retorno destas funções, permite saber se a função foi bem/mal-executada!):
int Exportar_Tabela_BDados_Excel(BDadosCoupe *BD, char *tabela, char *ficheir_csv) {
    return SUCESSO;
}

int Exportar_BDados_Excel(BDadosCoupe *BD, char *ficheir_csv) {
    return SUCESSO;
}

int Importar_BDados_Excel(BDadosCoupe *BD, char *ficheir_csv) {
    return SUCESSO;
}

int Exportar_BDados_Ficheiro_Binario(BDadosCoupe *BD, char *fich_dat) {
    return SUCESSO;
}

int Importar_BDados_Ficheiro_Binario(BDadosCoupe *BD, char *fich_dat) {
    return SUCESSO;
}

//L)	Apagar o conteúdo de uma Tabela. A Tabela continua a existir na BDados, mas não contém os dados, ou seja, os campos continuam, mas os registos são eliminados.
int DELETE_TABLE_DATA(TABELA *T) {
    if (!T)return INSUCESSO;
    if (T->LRegistos->NEL > 0) {
        NOG *node = T->LRegistos->Inicio;
        while (node) {
            printf("Removendo dados %s\n", node->Info);
            NOG *aux = node;
            free(aux->Info);
            node = aux->Prox;
            free(aux);

            T->LRegistos->NEL--;
        }
        return SUCESSO;
    }
    return INSUCESSO;
}


//M)	Apagar o conteúdo de uma Tabela e remove a tabela da base de dados.
int DROP_TABLE(BDadosCoupe *BD, char *nome_tabela) {
    if (!BD)return INSUCESSO;
    if (strlen(nome_tabela) < 1)return INSUCESSO;

    TABELA *t = Pesquisar_Tabela(BD, nome_tabela);
    if (t) {
        printf("Droping table %s\n", t->NOME_TABELA);
        //Only remove the data
        DELETE_TABLE_DATA(t);
        if (t->LCampos->NEL > 0) {
            while (t->LCampos->Inicio) {
                printf("Removendo campo %s\n", t->LCampos->Inicio->Info);
                NOG *node = t->LCampos->Inicio;

                free((CAMPO *) node->Info);

                t->LCampos->Inicio = node->Prox;
                free(node);
                t->LCampos->NEL--;
            }
            printf("Lista campos %d\n", t->LCampos->NEL);
            //todo - perceber pk este free da erro!
            // printf("Lista campos %",t->LCampos->Inicio);
            free(t->LCampos);

        } else {
            free(t->LCampos);
        }
        printf("elementos %d:\n", BD->LTabelas->NEL);
        //Remover tabela da lista Tabelas
        if (BD->LTabelas->NEL <= 1) {
            //Verificar se o unico elemento e o que procuramos
            NOG *n = BD->LTabelas->Inicio;
            printf("Tentando remover unico elemento\n");
            TABELA *aux = BD->LTabelas->Inicio->Info;
            if (strcmp(aux->NOME_TABELA, t->NOME_TABELA) == 0) {
                printf("Removendo unico elemento\n");
                free(aux);
                free(n);
                BD->LTabelas->NEL--;
                return SUCESSO;
            }
        } else {
            NOG *actual = BD->LTabelas->Inicio;
            while (actual) {
                NOG *next = actual->Prox;
                TABELA *atualT = (TABELA *) actual->Info;


                if (strcmp(atualT->NOME_TABELA, t->NOME_TABELA) == 0) {
                    printf("Found %s\n", atualT->NOME_TABELA);

                    //Nao existe um proximo
                    if (next == NULL) {
                        printf("matar");
                        free(actual);
                        BD->LTabelas->NEL--;
                        return SUCESSO;
                        //actual
                    } else {
                        printf("ccccccc");
                        actual->Prox = next->Prox;
                        free(actual);
                        BD->LTabelas->NEL--;
                        return SUCESSO;
                    }

                }
                actual = actual->Prox;
            }
            return INSUCESSO;
        }
        return SUCESSO;
    }
    return INSUCESSO;
}

//N)	Selecionar (Apresentar no ecran!) da base de dados todos os registos que obedeçam a uma dada condição, a função deve retornar o número de registos selecionados. (Ter em atenção o exemplo das aulas teóricas!). Nota: esta é certamente a funcionalidade mais usada num sistema de base de dados…, por isso se estiver bem otimizada…. O utilizador agradece!!!!
int
SELECT(BDadosCoupe *BD, char *_tabela, int (*f_condicao)(char *, char *), char *nome_campo, char *valor_comparacao) {
    return SUCESSO;
}

//O)	Remover todos os registos que obedeçam a uma dada condição, a função deve retornar o número de registos removidos.
int
DELETE(BDadosCoupe *BD, char *_tabela, int (*f_condicao)(char *, char *), char *nome_campo, char *valor_comparacao) {
    return SUCESSO;
}

//P)	Atualizar todos os registos da tabela onde o Campo é dado, que obedeçam a uma dada condição, a função deve retornar o número de registos que foram atualizados.
int UPDATE(BDadosCoupe *BD, char *_tabela, int (*f_condicao)(char *, char *), char *campo_comp, char *valor_campo_comp,
           char *nome_campo_update, char *valor_campo_update) {
    return SUCESSO;
}

