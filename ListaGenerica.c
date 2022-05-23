
#include "ListaGenerica.h"

ListaGenerica *CriarLG() {
    ListaGenerica *L = (ListaGenerica *) malloc(sizeof(ListaGenerica));
    L->Inicio = NULL;
    L->NEL = 0;
    return L;
}

void DestruirLG(ListaGenerica *L, void (*func)(void *)) {
    if (!L) return;
    if (!func) return;
    NOG *node = L->Inicio;
    NOG *Aux = NULL;
    while (node)
    {
        Aux = node->Prox;
        func (node->Info);
        free (node);
        node = Aux;
    }
    free(L);
}

/**
 *
 * @param L :Generic List
 * @param X : !Info! of the Node that will be added to the end of list
 * @return  : SUCESSO or INSUCESSO
 */
int AddLG(ListaGenerica *L, void *X) {
    if (!L)return INSUCESSO;
    if (!X)return INSUCESSO;
    if (L->Inicio == NULL) {
        //List is empty.Add to begin
        NOG *node = (NOG *) malloc(sizeof(NOG));
        node->Prox = NULL;
        node->Info = X;
        L->Inicio = node;
        L->NEL++;
    } else {
        //Add the Node to the end
        NOG *node = L->Inicio;
        //find the end of the list
        while (node->Prox) {
            node = node->Prox;
        }
        NOG *next = (NOG *) malloc(sizeof(NOG));
        next->Prox = NULL;
        next->Info = X;
        node->Prox = next;
        L->NEL++;
    }
    return SUCESSO;
}

void MostrarLG(ListaGenerica *L, void (*fshow)(void *)) {
    if (!L) {
        printf("%s", "List vazia\n");
        return;
    }
    //Passamos a lista para a funcao para tambem se conseguir ler o Nel da mesma.Util para desenhar a tabela
    fshow(L);

}
