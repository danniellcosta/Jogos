# 🏓 Pong em C++ com Raylib

## 📝 Descrição
Recriação do clássico jogo Pong com:
- **IA inteligente** com dificuldade progressiva
- Sistema de pontuação
- Efeitos sonoros
- Controles suaves via mouse

## 🕹️ Como Jogar
| Controle         | Ação                          |
|------------------|-------------------------------|
| Movimento do Mouse | Controla a raquete esquerda   |
| `ESPAÇO`         | Pausa/Despausa o jogo         |
| `ENTER`         | Reinicia após o game over     |

## 🛠️ Tecnologias Utilizadas
- **C++17**
- **Raylib 4.5** (renderização e áudio)
- **GCC/MinGW-w64** (compilação)

## ⚙️ Compilação e Execução

### Pré-requisitos
- [w64devkit](https://github.com/skeeto/w64devkit/releases) (já inclui Raylib)
- Ou Raylib instalado separadamente

### Passos
```bash
# Clone o repositório
git clone https://github.com/danniellcosta/Jogos.git
cd Pong

# Compile (usando w64devkit)
g++ -o bin/pong.exe src/*.cpp -Iinclude -lraylib -lwinmm -lgdi32 -lopengl32 -static

# Execute
cd bin
./bin/pong.exe
```