svgs='resources/svg'
images='resources/images'

color_a="rgb(200,200,200)"
color_b="rgb(150,150,150)"
color_c="#ff5e00"

function export_png() {
    svg=$1
    suffix=$2
    color=$3

    filename=$(basename $svg)
    filename=${filename::-4}

    svg_temp="$images/$filename"_temp_.svg
    png_temp="$images/$filename"_temp_.png
    output="$images/$filename"_"$suffix".png

    cp $svg $svg_temp

    # cambia el color 'fill'
    svg_temp
    sed -i "s|white|$color|g" $svg_temp
    #

    inkscape $svg_temp -e $png_temp

    ffmpeg -y -i $png_temp -vf scale=128:128 $output

    rm $svg_temp
    rm $png_temp

}

for svg in $svgs/*; do
    export_png $svg 'a' $color_a &>/dev/null
    export_png $svg 'b' $color_b &>/dev/null
    export_png $svg 'c' $color_c &>/dev/null
    echo $svg 'Exporting ...'
done
