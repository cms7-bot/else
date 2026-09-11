GUIA RÁPIDO — COMO BAIXAR E RODAR O JOGO

1. Baixe o projeto

Baixe/clonem o repositório:

https://github.com/cms7-bot/else

Se baixar em ZIP, extraia a pasta inteira antes de abrir.

Não mova jogo.exe, jogar.bat, Assets, Botoes, Cenarios, Code ou musicas para fora da pasta do projeto.

2. Instale o MSYS2

Instale o MSYS2 normalmente em:

C:\msys64

Depois abra o terminal MSYS2 UCRT64.

Atualize:

pacman -Syu

Se pedir para fechar o terminal, abra o MSYS2 UCRT64 novamente e rode:

pacman -Syu

3. Instale GCC e raylib

No terminal MSYS2 UCRT64:

pacman -S mingw-w64-ucrt-x86_64-gcc

Depois:

pacman -S mingw-w64-ucrt-x86_64-raylib

4. Abra o projeto no VS Code

Abra a pasta inteira do projeto no VS Code.

O terminal deve estar na raiz do projeto, por exemplo:

PS C:\Users\SeuNome\Desktop\else>

5. Verifique o GCC

No terminal do VS Code:

gcc --version

Se não funcionar:

$env:Path = "C:\msys64\ucrt64\bin;" + $env:Path

Depois teste novamente:

gcc --version

6. Verifique a raylib

Execute:

Test-Path "C:\msys64\ucrt64\include\raylib.h"
Test-Path "C:\msys64\ucrt64\lib\libraylib.a"

O resultado deve ser:

True
True

Se retornar False, volte ao passo 3.

7. Para compilar e executar

Depois de alterar e salvar o código, execute:

.\jogar.bat

O jogar.bat:

compila o código;

atualiza o jogo.exe somente se a compilação funcionar;

mantém o jogo.exe anterior se houver erro;

abre o jogo automaticamente.

O jogo.exe continua sendo o executável principal do projeto.

8. Fluxo normal de trabalho

Antes de começar:

git pull

Faça a alteração e salve.

Teste:

.\jogar.bat

Se estiver tudo correto:

git add .
git commit -m "descricao da alteracao"
git push

ERROS COMUNS

gcc não é reconhecido

Execute:

$env:Path = "C:\msys64\ucrt64\bin;" + $env:Path

raylib retorna False

Abra o MSYS2 UCRT64 e rode:

pacman -S mingw-w64-ucrt-x86_64-raylib

Assets, cenários ou músicas não aparecem

Confirme que está executando o projeto pela pasta raiz e use:

.\jogar.bat

O jogo não abre depois de alterar o código

Leia o erro mostrado pelo jogar.bat. Não apague arquivos nem reinstale bibliotecas sem verificar a mensagem primeiro.

IMPORTANTE

Sempre mantenha esta estrutura:

else/
├── Assets/
├── Botoes/
├── Cenarios/
├── Code/
├── musicas/
├── jogar.bat
└── jogo.exe

Para testar alterações, use sempre:

.\jogar.bat

Para apenas abrir a última versão já compilada:

.\jogo.exe