# Como contribuir com o ELSE

Obrigado por querer ajudar! Este guia mostra como preparar o ambiente, rodar o jogo e enviar uma contribuição.

## 1. Preparar o ambiente (Windows)

1. Instale o [MSYS2](https://www.msys2.org/) em `C:\msys64`.
2. Abra o terminal **MSYS2 UCRT64** e rode:
   ```
   pacman -Syu
   pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-raylib
   ```
3. Adicione `C:\msys64\ucrt64\bin` ao PATH do Windows.
4. Confira no terminal do VS Code: `gcc --version`

## 2. Rodar o jogo

1. Clone o repositório:
   ```
   git clone https://github.com/cms7-bot/else.git
   cd else
   ```
2. Compile e abra o jogo:
   ```
   .\jogar.bat
   ```
3. Deu erro? Veja o [Guia Rápido](./Guia_Rapido_ExecutarJogo.md).

Só quer jogar? Baixe o zip da última versão na aba [Releases](https://github.com/cms7-bot/else/releases), extraia e abra o `jogo.exe`.

## 3. Organização do código

| Pasta | O que tem |
|---|---|
| `Code/` | código fonte em C (raylib) |
| `Testes/` | testes automatizados das regras |
| `cenario/`, `assets/`, `Botoes/`, `HUD/` | imagens do jogo |
| `musicas/`, `EfeitosSonoros/` | áudio |
| `Documentação/` | histórias de usuário, requisitos, processo e casos de teste |

## 4. Fluxo de trabalho

1. Escolha um card no [board do Trello](https://trello.com/invite/b/6ab7228e40ff14f38be9f55b/ATTIa4223772b64eccef9d08a9bf0c63c73eED83464F/else-gestao-de-sprints-squad-8) e mova para **Em andamento**.
2. Atualize a main e crie uma branch a partir dela:
   ```
   git checkout main
   git pull
   git checkout -b tipo/descricao-curta
   ```
   Exemplos: `feat/maquina-cafe`, `fix/24-tela-promocao-por-cargo`, `dani/cantinhos`.
3. Faça commits pequenos no padrão **Conventional Commits**:
   ```
   feat(cartas): adiciona cartas do nível Pleno
   fix(promocao): corrige clique do botão Continuar
   docs(readme): atualiza a seção da Entrega 04
   ```
   Tipos: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`, `build`.
4. Teste no jogo antes de subir (`.\jogar.bat`).
5. Envie a branch e abra um **Pull Request** para a `main`:
   ```
   git push -u origin tipo/descricao-curta
   ```
6. Peça revisão de alguém do time. O merge só acontece depois da revisão.
7. Depois do merge, mova o card para **Concluído**.

**Nunca** faça push direto na `main` nem use `git push --force` nela. A `main` é protegida e só recebe código por Pull Request.

## 5. Reportar bugs

Abra uma [issue](https://github.com/cms7-bot/else/issues) com:
- o que aconteceu e o que deveria acontecer;
- como reproduzir (passo a passo);
- print ou vídeo, se possível;
- em qual cargo/fase do jogo aconteceu.

Ao corrigir, crie a branch `fix/<número>-descricao` e cite a issue no PR (`Closes #24`).
