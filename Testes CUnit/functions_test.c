#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/CUnit.h>
#include "bibliotecas/linked_list.c"
#include "bibliotecas/huffman.c"
#include "bibliotecas/binary_tree.c"

void test_create_linked_list()
{
    linked_list *list = create_linked_list(); //Cria a lista encadeada
    CU_ASSERT_PTR_NOT_NULL(list); //Checa se deixou de ser NULL
    CU_ASSERT_PTR_NULL(list->head); //Verifica se o ponteiro Head eh NULL
    CU_ASSERT_EQUAL(list->size, 0); // Verifica se o Size da lista ainda eh 0
}

void test_add_linked_list()
{
    linked_list *list = create_linked_list(); //Cria a lista
    add_linked_list(list, 10); //Adiciona o valor 10 ao ponteiro head da lista
    CU_ASSERT_PTR_NOT_NULL(list->head); //Verifica se o ponteiro Head deixou de ser NULL
    CU_ASSERT_EQUAL(list->head->valor, 10); // Checa se o valor foi adicionado corretamente
    CU_ASSERT_EQUAL(list->size, 1); // Verifica se o tamanho foi atualizado
    add_linked_list(list, 20); // Adiciona o valor 20 no inicio da lista
    CU_ASSERT_EQUAL(list->head->valor, 20); // Vrifica se o valor do inicio da lista atualizou para 20
    CU_ASSERT_EQUAL(list->size, 2); // Verifica se o tamanho foi atualizado
}

void test_add_linked_list_tail()
{
    linked_list *list = create_linked_list(); // Cria a lista encadeada
    add_linked_list_tail(list, 10); // Adiciona o valor 10 na lista
    CU_ASSERT_PTR_NOT_NULL(list->head); // Verifica se o ponteiro head deixou de ser NULL
    CU_ASSERT_EQUAL(list->head->valor, 10); // Verifica se o valor 10 realmente foi adicionado
    CU_ASSERT_EQUAL(list->size, 1); // Verifica se o tamanho atualizou para 1
    add_linked_list_tail(list, 20); // Adiciona o valor 20 no fim da lista
    CU_ASSERT_EQUAL(list->head->valor, 10); //Checa se o valor no inicio da lista permanece o 10
    CU_ASSERT_EQUAL(list->head->next->valor, 20); // Verifica se o valor do fim da lista eh 20
    CU_ASSERT_EQUAL(list->size, 2); // Verifica se o tamanho atualizou para 2
}

void test_remove_linked_node()
{
    linked_list *list = create_linked_list(); // Cria a lista
    add_linked_list(list, 10); // Adiciona 10 ao inicio
    add_linked_list(list, 20); // Adiciona 20 ao inicio
    list_node *removed_node = remove_linked_node(list); //Cria o ponteiro removed do tipo list_node
    CU_ASSERT_PTR_NOT_NULL(removed_node); //Checa se o ponteiro nao eh NULL
    CU_ASSERT_EQUAL(removed_node->valor, 20); //Checa se o valor atribuido ao ponteiro removed foi o 20
    CU_ASSERT_EQUAL(list->head->valor, 10); // Checa se o valor 10 permaneceu na lista
    CU_ASSERT_EQUAL(list->size, 1); // Checa se o tamanho foi atualizado para 1
}

void test_create_Huff()
{
    Huff *huff = create_Huff(); //Cria a estrutura huff
    CU_ASSERT_PTR_NOT_NULL(huff); //Checa se o ponteiro huff nao eh NULL
    CU_ASSERT_PTR_NULL(huff->head); // Checa se o ponteiro head de huff eh NULL
}

void test_is_empty()
{
    Huff *huff = create_Huff(); //Cria o ponteiro huff do tipo Huff
    CU_ASSERT_TRUE(is_empty(huff)); //Checa se a funcao retorna True/(1) para o ponteiro huff
}

void test_enqueue()
{
    Huff *huff = create_Huff(); //Cria o ponteiro huff do tipo Huff
    enqueue(huff, (void *)0x10, 2, NULL, NULL); //Enfileira o endereço de memória 0x10 
    CU_ASSERT_PTR_NOT_NULL(huff->head); //Verifica se o valor de head deixou de ser NULL
    CU_ASSERT_EQUAL(huff->head->frequencia, 2); //Checa se a frequencia foi adicionada corretamente
    CU_ASSERT_PTR_EQUAL(huff->head->byte, (void *)0x10); //Checa se o valor foi adicionado corretamente
}

void test_dequeue()
{
    Huff *huff = create_Huff(); //Cria o ponteiro huff tipo Huff
    enqueue(huff, (void *)0x10, 2, NULL, NULL); //Enfileira o endereco 0x10 com frequencia 2
    enqueue(huff, (void *)0x20, 3, NULL, NULL); //Enfileira o endereco 0x20 com frequencia 3
    CU_ASSERT(huff->head->frequencia == 2); // Checa se o primeiro da fila tem frequencia 2
    dequeue(huff); //Remove o primeiro da fila
    CU_ASSERT(huff->head->frequencia == 3); //Checa se o primeiro agora tem a sequencia 3, do endereco 0x20
}

void test_create_binary_tree()
{
    int *item = (int*) malloc(sizeof(int)); //Cria um ponteiro para um inteiro e aloca o espaço na memoria
    *item = 42;
    binary_tree *tree = create_binary_tree(item, NULL, NULL); //Chama a função para criar a arvore com um unico no que possui o item 42, e os ponteiros esquerda e direita NULL
    
    CU_ASSERT_EQUAL(*(int*)tree->item, 42); //Vai checar se os valores foram atribuidos corretamente
    CU_ASSERT_EQUAL(tree->left, NULL);
    CU_ASSERT_EQUAL(tree->right, NULL);
    
    free(item);
    free(tree);
}

int main()  
{
    // Inicializa o registro de suíte de testes do CUnit
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    // Adiciona a suíte de testes ao registro
    CU_pSuite suite = CU_add_suite("suite", 0, 0);

    // Adiciona os testes à suíte de testes
    CU_add_test(suite, "test_create_linked_list", test_create_linked_list);
    CU_add_test(suite, "test_add_linked_list", test_add_linked_list);
    CU_add_test(suite, "test_add_linked_list_tail", test_add_linked_list_tail);
    CU_add_test(suite, "test_remove_linked_node", test_remove_linked_node);
    CU_add_test(suite, "test_create_Huff", test_create_Huff);
    CU_add_test(suite, "test_enqueue", test_enqueue);
    CU_add_test(suite, "test_dequeue", test_dequeue);
    CU_add_test(suite, "test_is_empty", test_is_empty);
    CU_add_test(suite, "test_create_binary_tree", test_create_binary_tree);

    // Roda os testes
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
