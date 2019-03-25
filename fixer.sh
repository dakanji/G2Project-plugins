#!/bin/bash

PROJECT=$(php -r "echo dirname(realpath('$0'));")

PHP_CBF_BIN="$PROJECT/dev_vendor/bin/phpcbf"
PHP_CBF_CONFIG_START="$PROJECT/phpcs.xml"
PHP_CBF_CONFIG_END="$PROJECT/phpcs-end.xml"

PHP_FIX_BIN="$PROJECT/dev_vendor/bin/php-cs-fixer"
PHP_FIX_CONFIG="$PROJECT/.php_cs.dist"


PHP_FILES=$(git diff-index --name-only --cached --diff-filter=ACMRT HEAD -- | egrep '\.php$|\.inc$|\.class$')
FIX_FILES=$(git diff-index --name-only --cached --diff-filter=ACMRT HEAD -- | egrep '\.php$|\.inc$|\.class$|\.css$|\.js$|\.xml$|\.xaml$|\.html$')


FILES="$(find . -type f \( -name '*.php' -or -name '*.inc' -or -name '*.class' \)  | egrep -v '^(./lib/composer/|./dev_vendor/)')"
#FILES="$(find . -type f \( -name '*.php' -or -name '*.inc' -or -name '*.class' \) ! -path './lib/composer/*') ! -path './dev_vendor/*')"


for FILE in $FILES
do
echo "$FILE\n"
			# Prepare misc items in target file for PHPCBF and PHP-CS-FIXER
			# 39. Compress multiple blank lines
			RET=$(perl -0777 -p -i -e 's/\*\sThis program is free software; you can redistribute it and\/or modify\s*\n\s\*\sit under the terms of the GNU General Public License as published by\s*\n\s\*\sthe Free Software Foundation; either version 2 of the License, or \(at\s*\n\s\*\syour option\) any later version\.\s*\n\s\*\s*\n\s\*\sThis program is distributed in the hope that it will be useful, but\s*\n\s\*\sWITHOUT ANY WARRANTY; without even the implied warranty of\s*\n\s\*\sMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE\.  See the GNU\s*\n\s\*\sGeneral Public License for more details\.\s*\n\s\*\s*\n\s\*\sYou should have received a copy of the GNU General Public License\s*\n\s\*\salong with this program; if not, write to the Free Software\s*\n\s\*\sFoundation, Inc\., 51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA\./\* This program is free software; you can redistribute it and\/or modify it under the terms of\n \* the GNU General Public License as published by the Free Software Foundation;\n \* either version 2 of the License, or \(at your option\) any later version\.\n \*\n \* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;\n \* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE\.\n \* See the GNU General Public License for more details\.\n \*\n \* You should have received a copy of the GNU General Public License along with this program;\n \* if not, write to the Free Software Foundation, Inc\., 51 Franklin Street - Fifth Floor,\n \* Boston, MA  02110-1301, USA\./gms' $FILE)
			RET=$(perl -0777 -p -i -e 's/\*\sThis program is free software; you can redistribute it and\/or modify\s*\n\s\*\sit under the terms of the GNU General Public License as published by\s*\n\s\*\sthe Free Software Foundation; either version 2 of the License, or \(at\s*\n\s\*\syour option\) any later version\.\s*\n\s\*\s*\n\s\*\sThis program is distributed in the hope that it will be useful, but\s*\n\s\*\sWITHOUT ANY WARRANTY; without even the implied warranty of\s*\n\s\*\sMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE\.  See the GNU\s*\n\s\*\sGeneral Public License for more details\.\s*\n\s\*\s*\n\s\*\sYou should have received a copy of the GNU General Public License\s*\n\s\*\salong with this program; if not, write to the Free Software\s*\n\s\*\sFoundation, Inc\., 51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA\./\* This program is free software; you can redistribute it and\/or modify it under the terms of\n \* the GNU General Public License as published by the Free Software Foundation;\n \* either version 2 of the License, or \(at your option\) any later version\.\n \*\n \* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;\n \* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE\.\n \* See the GNU General Public License for more details\.\n \*\n \* You should have received a copy of the GNU General Public License along with this program;\n \* if not, write to the Free Software Foundation, Inc\., 51 Franklin Street - Fifth Floor,\n \* Boston, MA  02110-1301, USA\./gms' $FILE)
done
