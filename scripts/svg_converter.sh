# icono svg bajados en:
# https://material.io/resources/icons/?style=baseline
source utils/shell/util.sh

svgs='resources/svg'
images='resources/images'

disable_color="$(jread stylesheet/palette.json 'b30')"
normal_color="$(jread stylesheet/palette.json 'b60')"
hi_color="$(jread stylesheet/palette.json 'b80')"
checked_color="$(jread stylesheet/palette.json 'base')"

size=128

mkdir $images

function export_png() {
    svg=$1
    suffix=$2
    color=$3

    filename=$(basename $svg)
    filename=${filename::-4}

    svg_temp="$images/$filename"_temp_.svg
    output="$images/$filename"_"$suffix".png

    if [ ! -f $output ]; then
        cp $svg $svg_temp

        # cambia el color 'fill'
        svg_temp
        sed -i "s|fill=\"none\"|fill=\"$color\"|g" $svg_temp
        sed -i "s|fill:none|fill:$color|g" $svg_temp
        sed -i "s|stroke:none|stroke:$color|g" $svg_temp
        #

        # cambia tamaño
        sed -i "s|height=\"24\"|height=\"$size\"|g" $svg_temp
        sed -i "s|width=\"24\"|width=\"$size\"|g" $svg_temp
        #
        #

        inkscape $svg_temp -e $output

        rm $svg_temp
    fi

}

for svg in $svgs/*; do
    export_png $svg 'normal' $normal_color &>/dev/null
    export_png $svg 'disable' $disable_color &>/dev/null
    export_png $svg 'hi' $hi_color &>/dev/null
    export_png $svg 'white' 'white' &>/dev/null
    export_png $svg 'checked' $checked_color &>/dev/null
    echo $svg 'Exporting ...'
done
