# Abelian-sandpile-model
Консольное приложение - упрощенная модель песчаной кучи, которая позволяет сохранять свои состояния в картинку в формате BMP без использования стандартных контейнеров.

Изначальное состояние задается входным файлом.

Размер сетки может изменяться в процессе работы программы.

Возможно следующие аргументы командной строки:

-i, --input - tsv-файл (tab-separated values) c начальными данными

-o, --output - путь к директории для сохранения картинок

-m, --max-iter - максимальное количество итераций модели

-f, --freq - частота, с которой должны сохранятся картинки (если 0, то сохраняется только последнее состояние)

# Начальное состояние

Начальное состояние задается файлом со значением количества песчинок в каждой ячейке, кроме пустых. Размер сетки следует рассчитать на основании этих данных - минимальный прямоугольник в который попадают все ячейки.

Формат файла: Каждая строчка содержит информацию об одной ячейке, в виде (x-координаты, y-координаты, количество песчинок), разделенных символом табуляции. Количество песчинок гарантированно влезет в uint64_t, координаты гарантированно влезают в int16_t

# Примечания к модели

Новые песчинки добавляются только при инициализации.

Состояние следующего поколения ячеек зависит только от предыдущего состояния сетки.

В случае если песчинки пытаются обвалиться за границу сетки, ее размер увеличивается на 1 в соответствующую сторону.

Программа должна пересчитывать состояние модели согласно начальным данным, а также сохранять промежуточные состояния с заданной частотой в виде картинки в формате bmp.

Картинка для текущего состояния формируется по следующим правилам:

Размер картинки равен размеру поля.

Каждый пиксель соответствует ячейке поля.

Цвет пикселя зависит от количества песчинок в ячейке.

0 - белый

1 - зеленый

2 - желтый

3 - фиолетовый

4+ - черный

Кодирование 1 пикселя должно занимать не более 4 бит.

Программа должна закончить свою работу в случае если модель достигла стабильного состояния, либо номера заданной изначально итерации.
