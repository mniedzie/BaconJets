file_name=controlplotsBC_trgM_01Dec
if [ "$#" = "1" ]
   then file_name=controlplots${1}_trgM_01Dec
fi
rm ${file_name}.tex
py_out=$(./generate_tex_trgMatched.py "$file_name")
pdflatex ${file_name}.tex
rm ${file_name}.[tosnla][^e]*
echo -e "\nthe python output was:"
echo "$py_out"
