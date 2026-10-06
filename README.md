# Sistema-Empresarial
Este trabalho prático final tem como objetivo consolidar os pilares da Programação Estruturada aplicados ao desenvolvimento de uma aplicação desktop em modo terminal.

# Objetivos Pedagógicos:
- Mapear entidades do mundo real em estruturas de dados compostas (struct).
- Dominar a passagem de parâmetros por referência através de ponteiros para garantir
eficiência e modularidade.
- Utilizar leitura e escrita em baixo nível, otimizando o tempo de E/S e permitindo o
acesso direto (random access) através de movimentação do ponteiro de arquivo
(fseek).
- Desenvolver sub-rotinas coesas, com responsabilidade única e baixo acoplamento.

- # 🍺 Sistema de Gerenciamento

Um sistema de gerenciamento de estoque em **C++**, executado no console, criado para ajudar pequenos comércios, como bares e lanchonetes, a manter seus produtos organizados de forma simples e rápida.

Sem banco de dados, sem instalação complicada: os dados ficam salvos em um arquivo binário (`Produtos.dat`) e o programa funciona direto no terminal.

---

## ✨ Funcionalidades

| Opção | O que faz |
|-------|-----------|
| **Cadastrar** | Registra novos produtos com código gerado automaticamente |
| **Relatório** | Lista todos os produtos em formato de tabela |
| **Pesquisar** | Localiza um produto pelo código e exibe seus detalhes |
| **Editar** | Altera descrição, quantidade, data de compra e preços |
| **Excluir** | Remove um produto, com confirmação antes de apagar |

## 📦 Dados armazenados por produto

- Código (sequencial e automático)
- Descrição
- Quantidade em estoque
- Data da compra
- Preço de custo
- Preço de venda

## 🛠️ Tecnologias e conceitos aplicados

- **C++** (biblioteca padrão)
- **Manipulação de arquivos binários** (`fopen`, `fread`, `fwrite`, `fseek`, `ftell`)
- **Structs** para modelagem dos registros
- **Modularização** em funções
- Operações de **CRUD** completas (Create, Read, Update, Delete)

> ⚠️ O projeto utiliza comandos do Windows (`system("cls")` e `system("pause")`), portanto foi pensado para rodar no **Windows**.

## 💡 Motivação

Este projeto nasceu como um exercício prático de programação, com um objetivo real: resolver o problema de controle de estoque de um bar, mostrando que dá para construir uma ferramenta útil usando apenas os fundamentos da linguagem.

## 👥 Autores

Desenvolvido por **Gabriel G. Minatel**.

---
⭐ Se o projeto foi útil ou interessante, deixe uma estrela no repositório!
