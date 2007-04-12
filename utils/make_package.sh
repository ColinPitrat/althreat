#!/bin/bash

if [ -z $2 ]
then
    echo "Syntaxe: $0 <archive file> <package file>"
fi

ARCHIVE_FILE=$1
PACKAGE_FILE=$2

cat << EOF > $PACKAGE_FILE
#!/bin/bash

TIMESTAMP=\`date +%s\`
ARCHIVE=/tmp/AlThreat_\$TIMESTAMP.tar.gz
BEGIN_LINE=\`grep -n BEGINNING\ OF\ PACKAGE \$0| awk -F : '{ print \$1 }'\`
END_LINE=\`wc -l \$0 | awk '{ print \$1 }'\`
let SIZE=\$END_LINE-\$BEGIN_LINE
tail -n \$SIZE \$0 | base64 -d - > \$ARCHIVE

AlThreat_install \$ARCHIVE

exit
# --- BEGINNING OF PACKAGE ---
EOF

base64 $ARCHIVE_FILE >> $PACKAGE_FILE

chmod a+x $PACKAGE_FILE
