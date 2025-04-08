# Назва кінцевого виконуваного файлу
TARGET = bin/lg

# Компілятор
CC = gcc

# Флаги компілятора
CFLAGS = -Wall -Wextra -Iinclude -O3
# Флаги бібліотек
LDFLAGS = -lm -lunistring

# Шлях до директорій
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Список C-файлів
SRCS = $(wildcard $(SRCDIR)/*.c)

# Список відповідних об'єктних файлів
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# Компіляція всього проєкту
all: $(TARGET)

# Лінкування об'єктних файлів у виконуваний файл
$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Компіляція окремих C-файлів у об'єктні файли
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка зібраних файлів
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Фіктивні цілі, щоб Make не сприймав їх за файли
.PHONY: all clean
