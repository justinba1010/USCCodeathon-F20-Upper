SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd -P)"

mkdir -p input
mkdir -p output

pushd $SCRIPT_DIR
  values=(
  '2,3,2'
  '3,5,2'
  '4,10,2'
  '5,10,3'
  '6,20,3'
  '7,40,3'
  '8,80,3'
  '9,250,3'
  '10,500,3'
  '11,1000,3'
  '12,10,4'
  '13,20,4'
  '14,40,4'
  '15,100,4'
  '16,10,5'
  '17,20,5'
  '18,40,5'
  '19,10,6'
  '20,20,6'
  '21,10,7'
  '22,10,8'
  '23,10,9'
  '24,100,3'
  '25,100,5'
  '26,10000,3'
  '27,20000,3'
  '28,400,4'
  '29,1000,4'
  '30,100,5'
  '31,1000,5'
  #'32,1000000,5'
  )

  for datarow in "${values[@]}"; do
    while IFS=',' read -r i n k;  do
      echo $i $n $k
      echo $n $k | python3 ./mkin.py > input/input$i.txt;
      python3 ./solutions/solution.py < input/input$i.txt > output/output$i.txt
    done <<< "$datarow"
  done

  for i in {0..1}
  do
    python3 ./solutions/solution.py < input/input$i.txt > output/output$i.txt
  done
popd

rm -rf cases.zip
zip -r cases input output
