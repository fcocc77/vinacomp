#  crea una paleta de colores en un archivo sass para los estilos y
#  la misma paleta en un archivo json para poder usarla dentro de 'qt'
from colour import Color
import sys
sys.path.insert(1, './utils/python')
from util import jwrite, fwrite

hue = 0.6
saturation = 0.1

palette = {}

#  Paleta para 'json'
for i in range(100):
    luminance = (i + 1) / 100
    palette['b' + str(i)] = Color(hue=hue, saturation=saturation, luminance=luminance).hex

palette['base'] = '#ffcc00'
palette['ghost_base'] = '#917400'

#  colores para los textos
palette['c0'] = palette['b60']
palette['c1'] = palette['b80']
palette['c2'] = palette['b90']
palette['c3'] = palette['b95']

jwrite('./stylesheet/palette.json', palette)


#  Paleta para 'sass'
sass_palette = ''
for key, value in palette.items():
    sass_palette += '$' + key + ': ' + value + '\n'

fwrite('./stylesheet/palette.sass', sass_palette)
