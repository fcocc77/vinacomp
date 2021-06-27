# icono svg bajados en:
# https://material.io/resources/icons/?style=baseline
source utils/shell/util.sh
source utils/shell/svg2png.sh

svgs='resources/svg'
images='resources/images'

disable_color="$(jread stylesheet/palette.json 'b30')"
normal_color="$(jread stylesheet/palette.json 'b60')"
hi_color="$(jread stylesheet/palette.json 'b80')"
checked_color="$(jread stylesheet/palette.json 'base')"

mkdir $images

for svg in $svgs/*; do
    svg_to_png "$images" $svg 'normal' $normal_color &>/dev/null
    svg_to_png "$images" $svg 'disable' $disable_color &>/dev/null
    svg_to_png "$images" $svg 'hi' $hi_color &>/dev/null
    svg_to_png "$images" $svg 'white' 'white' &>/dev/null
    svg_to_png "$images" $svg 'checked' $checked_color &>/dev/null
    echo $svg 'Exporting ...'
done
