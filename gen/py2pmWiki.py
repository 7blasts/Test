<<<<<<< HEAD
#!/usr/bin/python
# coding=utf-8
from __future__ import print_function

import os
import re
import json
import time
import ftplib
import traceback

python_code_mark_regexp = re.compile("^\s*#\s?WIKI\.(\S*?):(.*)$")
python_code_part_regexp = re.compile("^\s*#(.*)$")
wiki_python_mark_regexp = re.compile("(^\(:gen_from_python\s*(.*):\))(.*)$")
wiki_page_groups_regexp = re.compile("^\s*(\S*)\s*$")
has_forward_empty_space = re.compile("^(\s\s*)(.*)$")
wiki_cpp_mark_regexp = re.compile("(^\(:gen_from_cpp\s*(.*):\))(.*)$")
is_empty_line_regexp = re.compile("^(\s*)$")

pmWiki_gen_begin = "^(:\s*comment\s+PYTHON.GEN_UPLOADS_HERE_BEGIN\s*:).*$"
pmWiki_gen_end   = "^(:\s*comment\s+PYTHON.GEN_UPLOADS_HERE_END\s*:).*$"

# WIKI.PAGE: gen/src/scr/utils/py2pmWiki.py
# WIKI.HEAD: Данный модуль предназначен для преобразования исходного кода
# языков программирования, используемых в проекте, в исходный код '''pmWiki'''.
# А также для обратного преобразования исходного кода '''pmWiki''' страницы
# в код исходного языка программирования.

# WIKI.HEAD: !!!Мотивация
# *[[Main.DevelopmentConvey|В раздел "Цикл разработки": \
# В чем заключается производственный процесс...]]
# *[[Main.DevelopersWiki|В раздел "Среда разработки": Роль pmWiki в данном \
# проекте...]]
#
# Эта часть конвейера автоматизации существенно улучшает читаемость кода и его
# документации, за счет возможности включения в исходный код программы вики-\
# маркапов для вставки графических схем и гиперссылок на любые интернет
# документы необходимые для понимания устройства данного кода.
#
# (:footnote:)
# Важно знать техническое задание, используемые интерфейсы, умолчания и
# ограничения. Нужно знать координаты автора данного кода, для быстрой связи
# с ним в случае если код перестал компилироваться, работать или обнаружилась
# другая его уязвимость. Получив пароль доступа к соответствующей странице
# проекта, автор сможет немедленно внести поправки в свой код из ближайшего
# интернет кафе, используя редактор встроенный в вики. Описание этого варианта
# правки кода объясняет необходимость реализации преобразования вики текста
# в исходный код языка программирования.
# (:footnoteend:)
#
# Кроме прочего, удаление исходного файла из проекта автоматически удалит и
# размещённую в нём документацию. Это избавляет от работ по ручной
# синхронизации двух параллельных иерархий: кода и документации к коду.
#
# В случае изменения алгоритма внутри исходника, документация, которую нужно
# поправить, расположена рядом — в этом же файле. Это избавляет от поиска
# страниц документации, в которые эти правки нужно внести.
#
# !!!Реализация.
# Реализация затруднена тем, что весь первоначальный загрузчик проекта на
# машину пользователя должен, в идеале, являться единым исходником, который
# и нужно забрать средствами первого попавшегося браузера для разворачивания
# всех остальных исходных файлов уже из вики. Где-то в вики должна быть папка
# с неупакованным в вики исходником, содержащая такой загрузчик. Для запуска
# которого, достаточно проинсталлировать на машину Python.
#
# Для успешной реализации декодера Wiki => src, с доступом через TCP или
# HTTP ( не HTTPS! ), нужно помнить роль VTSS в RTS. А именно то, что члены
# VTSS - автоматы состояний с сильными взаимными связями и высокой частотой
# передачи управления главному диспетчеру событий (требования RTS:—синхронная
# гостиница для синхронных ботов). Декодер Wiki => src не обязательно делать
# синхронным ботом, но такое развитие событий не исключено. Бот VTSS, который
# отслеживает вики-страницу может оказаться полезным элементом системы отладки
# как источник сигнала для быстрой реакции на изменение содержимого страницы.
# Например при отладке шейдеров, сразу после сохранения шейдера бот сможет
# автообновить шейдер на стороне видео, за счет ежесекундного сканирования
# страницы. Но в релизе такое сканирование, максимум ежедневное. То-же
# касается и обновления файлов с геометрией, текстурами и т.п. Само собой, что
# такое решение не верно и избыточо, что нужен механизм посылающий запрос
# каждое сохранение страницы на IP определённого VTSS, к которому подписан
# UISS начинающий по этому сигналу обновление данных на стороне GPU.


# WIKI.TEXT: !!!Класс WikiFile
# Предназначен для преобразования файла из pmWiki-формата в исходный файл на
# Python, C/C++, GLSL, ... в зависимости от наличия вики файле однострочного
# маркапа @@wiki_source@@ и его аргументов
class WikiFile(object):
    """pmWiki object created from pmWiki source file"""
    # Это обычный комментарий не конвертируемый в вики текст
    def __init__(self, wiki_source):
        """Create new WikiFile instance
        :param wiki_source: unicode object with pmWiki page source text
        """
        self.wiki = wiki_source  # pmWiki source text
        self.role = u'wiki'      # wiki|python|c|c++|glsl
        self.code = u''          # data or code extracted from wiki source
        self.page_title = u"Title.ext"
        self.page_group = u"Group"

    def as_wiki_source(self):
        """Real method as_wiki_source must be overridden in derived classes
        :return: target wiki-page's source text
        """
        return self.__class__.as_wiki_source.__doc__

    def page_file_name(self):
        return self.page_group + u"." + self.page_title
# ...
    # WIKI.TEXT: !!!Статические методы WikiFile:
    # *'''@@.path_to_group(путь к папке) =>@@''' ''имя группы в вики''
    # *'''@@.name_to_title(имя файла   ) =>@@''' ''имя страницы в вики''
    # *'''@@.group_to_path(имя группы  ) =>@@''' ''исходный путь к папке''
    # *'''@@.title_to_name(имя страницы) =>@@''' ''исходное имя файла''

# ... class WikiFile
    @staticmethod
    def path_to_group(path):
        g = os.path.abspath(os.path.normpath(path))
        g = g.replace("\\", "/")
        g = "Dir".join((g.split("/")))
        g = g.replace("_", "Sub")
        g = g.replace(".", "Dot")
        return g

    @staticmethod
    def group_to_path(g):
        g = g.replace("Dot", ".")
        g = g.replace("Sub", "_")
        g = "/".join(g.split("Dir"))
        return g

    @staticmethod
    def name_to_title(name):
        t, ext = os.path.splitext(name)
        t = t.replace("_", "Sub")
        t = t.replace(".", "Dot")
        if ext:
            t = "Gen" + t + "Gen" + ext[1:]
        else:
            t = "Gen" + t + "Gen"
        return t

    @staticmethod
    def title_to_name(generated_title):
        name, ext = generated_title.rsplit("Gen", 1)
        name = name.split("Gen", 1)[1]
        name = name.replace("Dot", ".")
        name = name.replace("Sub", "_")
        if ext:
            return ".".join((name, ext))
        return name

# ...
    # WIKI.TEXT: !!!Метод WikiFile.wiki_src_to_wiki_file
    # Сохраняет вики-страницу в файл, пригодный для переноса на pmWiki сайт.
    # Файл будет размещён в текущей рабочей директории.
    # Читать [[http://www.pmwiki.org/wiki/PmWiki/PageFileFormat\
    # |описание формата файла '''pmWiki''']] ...
    # Если имя файла не указано, то используется имя возвращаемое методом
    # '''@@page_file_name@@'''
    # Записывает текущее время в поле '''@@ctime@@''' - "время создания" файла.
    # Записывает имя '''@@"admin"@@''' в поле '''@@author@@''' и имя файла в
    # поле '''@@name@@'''

# ... class WikiFile
    def save_to_file(self, file_name=None):
        if not file_name:
            file_name = self.page_file_name()
        with open(file_name, 'w') as out:
            src = self.as_wiki_source()
            src = src.replace("%", "%25")
            src = src.replace("\n", "%0a")
            out.write("version=pmwiki-2.2.70 ordered=1 urlencoded=1\n")
            out.write("agent=Mozilla/5.0 (X11; U; Linux i686; ")
            out.write("en-US; rv:1.8.1.19) Gecko/20081216 Ubuntu/8.04 (hardy)")
            out.write(" Firefox/2.0.0.19\n")
            out.write("author=admin\n")
            out.write("charset=UTF-8\n")
            #  out.write("csum=mv\n")
            out.write("title=%s\n" % self.page_title)
            out.write("name=%s\n" % self.page_file_name())
            out.write("ctime=%d\n" % int(time.time()))
            out.write("text=")
            out.write(src.encode('utf8'))
            out.write("\n")
            out.close()

    def parse_markups(self, src):
        # Любой вики файл мог быть сгенерирован автоматически из
        # исходного кода на другом языке программирования:
        # wiki_python_mark_regexp
        # wiki_c_mark_regexp
        # wiki_cpp_mark_regexp
        # wiki_glsl_mark_regexp
        pass


# WIKI.TEXT: !!!Класс WikiGenText
# Генерируемый блок текста в вики формате. Предназначен для накопления строк
# вики текста, на промежуточном этапе генерации вики-текста из исходного кода
class WikiGenText(object):
    def __init__(self):
        self.lines = []

    def append(self, line):
        clean = self.clean_line(line)
        if isinstance(clean, str):
            self.lines.append(clean.decode("utf-8"))
        elif isinstance(clean, unicode):
            self.lines.append(clean) 

    def is_empty(self):
        return not len(self.lines)

    def as_wiki_source(self):
        return '\n'.join(self.lines)

    @staticmethod
    def clean_line(line):
        # has_forward_empty_space = re.compile("^(\s\s*)(.*)$")
        match = has_forward_empty_space.match(line)
        if match:
            line = match.groups()[1]
        return line


# WIKI.TEXT: !!!Класс WikiGenCode
# Генерируемый блок исходного кода программы в вики-формате. Предназначен для
# накопления строк фрагмента исходного кода на промежуточном этапе генерации
# вики-текста внутри многострочного маркапа "source"
class WikiGenCode(object):
    def __init__(self):
        self.lines = []
        self.empty_tail_lines_count = 0

    def append(self, line):
        if isinstance(line, str):
            self.lines.append(line.decode("utf-8"))
        elif isinstance(line, unicode):
            self.lines.append(line) 

    def is_empty(self):
        return not len(self.lines)

    def as_wiki_source(self):
        len_lines = len(self.lines)
        for r in (-i-1 for i in xrange(len_lines)):
            match = is_empty_line_regexp.match(self.lines[r])
            if match:
                self.empty_tail_lines_count += 1
            else:
                break
        code_lines = len_lines - self.empty_tail_lines_count
        wiki_lines = u""
        if code_lines:
            head = u'(:sour'+u'ce lang=python -getcode -trim:)[@'
            tail = u'@'+u']\n'
            body = u"\n".join(self.lines[:code_lines])
            wiki_lines = head + body + tail
        wiki_lines += u"\n".join(self.lines[code_lines:])+u"\n"
        return wiki_lines


# WIKI.TEXT: !!!Класс WikiFileFromPython
# Предназначен для создания вики-текста из исходного файла на языке
# программирования '''Python'''
class WikiFileFromPython(WikiFile):
    """pmWiki object created from Python source file"""
    def __init__(self, python_source):
        """Create new WikiFile instance from Python source
        :param python_source: Python source, possible with wiki markups
        """
        WikiFile.__init__(self, u"")
        self.role = u'python'
        self.code = python_source.split('\n')
        self.code_len = len(self.code)
        # Временные переменные для коллекционирования python-markups
        self.head = []               # optional, pre-code wiki source
        self.tail = []               # optional, post-code wiki source
        self.body = [WikiGenCode()]  # each line without markup is a code line
        self.parse_python_markups()

    def as_wiki_source(self):
        result = u""

        result += u"(:comm"+u"ent "+u"parsed head::)\n"
        for i in self.head:
            result += i.as_wiki_source() + u'\n'

        result += u"(:comm"+u"ent "+u"parsed body::)\n"
        for i in self.body:
            result += i.as_wiki_source() + u'\n'

        result += u"(:comm"+u"ent "+u"parsed tail::)\n"
        for i in self.tail:
            result += i.as_wiki_source() + u'\n'

        result += u"(:comm"+u"ent "+u"parsed stop::)\n"
        return result
# ...
    # WIKI.TEXT:!!!Метод WikiFileFromPython.parse_python_markups:
    #
    # Осуществляет поиск маркеров вики-вставок в комментариях к исходному коду
    # Python. Парсер рассматривает исходный текст построчно. Если строка
    # начинается с '''@@'# WIKI.ИМЯ_МЕТОДА:'@@''', при этом лидируюшие пробелы
    # игнорируются, то будет произведена проверка наличия метода с именем
    # '''@@'gen_p2w_имя_метода'@@''' у данной инстанции класса. Если метод
    # имеется в наличии, то парсер вызовет его, иначе — проигнорирует
    # совпадение и будет обрабатывать эту строчку, как очередную строчку
    # исходного кода.\

# ... class WikiFileFromPython
    def parse_python_markups(self):
        line = 0                         # current parsed line number
        while line < self.code_len:
            match = python_code_mark_regexp.match(self.code[line])
            if match:
                parse_method = "gen_p2w_%s" % match.groups()[0].lower()
                if hasattr(self, parse_method):
                    accepted = getattr(self, parse_method)(match, line)
                    if accepted:
                        line += accepted
                        continue
            # No regexp match, or no method, or not accepted?
            # — So, current line is part of last body's code block.
            # If last block is not some code block, create new one:
            curr_block = self.body[-1]
            if not isinstance(curr_block, WikiGenCode):
                curr_block = WikiGenCode()
                self.body.append(curr_block)
            # TODO: append current line to current code block
            curr_block.append(self.code[line])
            line += 1
        # On stop iteration lines.

    # WIKI.TEXT: !!!Метод WikiFileFromPython.gen_p2w_page
    #
    # Парсер однострочной метки '''@@WIKI.PAGE@@''' — наименования файла в
    # базе знаний. Если разбор строки прошел успешно, метод возвращает 1,
    # сообщая о том что принял одну строку текста. Иначе возвращает 0,
    #  тем самым вынуждая парсер обработать строку как обычную строку кода.\

# ... class WikiFileFromPython
    def gen_p2w_page(self, match, line):
        """Parse markup 'WIKI.PAGE' — single line markup with wiki-page name.
        For example: # WIKI.PAGE: PYTHON/utils/Group/py2pmWiki.py
        Generate page name "PythonUtilsGroup.py2pmWiki_gen_py".
        :param match: python_code_mark_regexp.match object with data groups
        :param line: index of current parsed line in source code
        :return count of accepted lines or None if line not accepted
        """
        path_match = wiki_page_groups_regexp.match(match.groups()[1])
        if path_match:
            print("%04d|>>>gen_p2w_page" % line, path_match.groups())
            path = path_match.groups()[0]
            pth = path.split('/')
            self.page_group = str().join([i.lower().title() for i in pth[:-1]])
            self.page_title = pth[-1]  # .rsplit(".", 1)[0]
            print("page_file_name =", self.page_file_name())
            return 1
        # return None is like return 0

    # #WIKI.NAME: Однострочная метка — заголовок страницы в базе знаний.
    def gen_p2w_name(self, match, line):
        print("%04d|>>>gen_p2w_name" % (line + 1), match.groups(), self)
        return 1

    # #WIKI.HEAD: Многострочня метка — содержит параграф вики-страницы,
    # который выносится за пределы кода, и располагается выше кода, но ниже
    # предыдущего параграфа созданного аналогичным тегом.
    def gen_p2w_head(self, match, line):
        print("%04d|>>>gen_p2w_head" % (line + 1), match.groups(), self)
        curr_block = WikiGenText()
        curr_block.append(match.groups()[1])
        self.head.append(curr_block)
        next = line+1
        accepted = 1
        while next <= self.code_len:
            next_match = python_code_part_regexp.match(self.code[next])
            if next_match:
                print("----|>>>gen_p2w_text", next_match.groups()[0])
                curr_block.append(next_match.groups()[0])
                accepted += 1
                next += 1
                continue
            else:
                break
        curr_block.append("\n")
        return accepted

    # WIKI.TAIL: Многострочня метка — содержит параграф вики-страницы,
    # который выносится за пределы исходного кода, и располагается ниже кода
    # и ниже любого предыдущего параграфа созданного аналогичным тегом.
    def gen_p2w_tail(self, match, line):
        print("%04d|>>>gen_p2w_tail" % (line + 1), match.groups(), self)
        return 1

    # WIKI.TEXT: Многострочня метка — содержит параграф вики-страницы,
    # который рассекает текщий блок исходного кода, либо является следующим
    # параграфом в предыдущем рассечении
    def gen_p2w_text(self, match, line):
        curr_block = self.body[-1]
        if isinstance(curr_block, WikiGenCode):
            if curr_block.is_empty():
                # curr block is empty code block
                # TODO: Try switch to prev block "self.body[-2]"
                if len(self.body)>1:
                    curr_block = self.body[-2]
                else:
                    code_head = self.body.pop(-1)
                    curr_block = WikiGenText()
                    self.body.append(curr_block)
                    self.body.append(code_head)
            else:
                # curr block is code block and it is not empty:
                curr_block = WikiGenText()
                self.body.append(curr_block)
        assert(isinstance(curr_block, WikiGenText))
        last_block_line = len(curr_block.lines)
        # TODO: append current line to current code block
        print("%04d|>>>gen_p2w_text\n" % (line+1), self.code[line])
        curr_block.append(u"\n")
        curr_block.append(match.groups()[1])
        next = line+1
        accepted = 1
        while next < self.code_len:
            next_match = python_code_part_regexp.match(self.code[next])
            if next_match:
                print("----|>>>gen_p2w_text", next_match.groups()[0])
                curr_block.append(next_match.groups()[0])
                accepted += 1
                next +=1
                continue
            else:
                break
        return accepted


# WIKI.TEXT: !!!Тесты и примеры использования.
#
# Если модуль используется как основной файл проекта,
# то запускается процесс его тестирования.
#
# Тестовый код содержит и примеры использования.
if __name__ == '__main__':
    import sys

    # WIKI.TEXT:!!!Отправка результирующих файлов на pmWiki сайт проекта
    # Конечно ваш собственный список ftp сайтов и паролей к ним может быть
    # организован по другому, более разумно, но для простоты будем полагать
    # что список ftp сайтов и паролей к ним находится в файле @@mirrors.json@@
    # в текущей рабочей директории (@@current working directory@@).
    # И организован как '''@@JSON@@''' список элементов вида:
    # (:source lang=python -getcode:)[@
    # [{"ftp":"ftp.myWiki0.net", "usr":"name0", "pwd": "p0", "dir":["aa","d"]}
    # ,{"ftp":"a30.pmWiki1.net", "usr":"name1", "pwd": "p1", "dir":["d","d"]}
    # ,{"ftp":"g11.pmWiki2.net", "usr":"name2", "pwd": "p2", "dir":["d"]}
    # ]
    # @]
    # Устройство файла @@mirrors.json@@:
    # *Поле "ftp" — содержит имя FTP-сервера
    # *Поле "usr" — содержит логин
    # *Поле "pwd" — содержит пароль
    # *Поле "dir" — содержит путь к целевой папке в виде списка каталогов
    #
    def ftp_update_mirrors(_file_name_mirrors, _file_name):
        def ftp_src_upload(_ftp, __file_name):
            _ftp.storlines(
                "STOR " + __file_name, open(__file_name)
            )
        # def ftp_bin_upload(_ftp, __file_name):
        #     _ftp.storbinary(
        #         "STOR " + __file_name, open(__file_name, "rb"), 1024
        #     )
        _failed = []
        with open(_file_name_mirrors) as mirrors_file:
            mirrors_json = mirrors_file.read()
            mirrors_list = json.loads(mirrors_json)
        mirrors_len = len(mirrors_list)
        print("="*10+"<FTP MIRRORS UPDATE>"+"="*10)
        for mirror in mirrors_list:
            try:
                print(mirror)
                ftp = ftplib.FTP(mirror["ftp"])
                ftp.login(mirror["usr"], mirror["pwd"])
                for folder_name in mirror["dir"]:
                    ftp.cwd(folder_name)
                ftp_src_upload(ftp, _file_name)
            except:  #TODO: Indicate more concrete upload error
                exc = traceback.format_exc()
                _failed.append((mirror, exc))
        print("="*10+"<MIRRORS UPDATE END>"+"="*10)
        print("| UPDATED %s FROM %s FTP mirrors" % (
            mirrors_len - len(_failed), mirrors_len
        ))
        return _failed

    # WIKI.TEXT:!!!Отправка результирующих файлов на pmWiki сайт проекта
    # Конечно ваш собственный список ftp сайтов и паролей к ним может быть
    # организован по другому, более разумно, но для простоты будем полагать
    # что список ftp сайтов и паролей к ним находится в файле @@mirrors.json@@
    # в текущей рабочей директории (@@current working directory@@).
    # И организован как '''@@JSON@@''' список элементов вида:
    # (:source lang=python -getcode:)[@
    # [{"ftp":"ftp.myWiki0.net", "usr":"name0", "pwd": "p0", "dir":["aa","d"]}
    # ,{"ftp":"a30.pmWiki1.net", "usr":"name1", "pwd": "p1", "dir":["d","d"]}
    # ,{"ftp":"g11.pmWiki2.net", "usr":"name2", "pwd": "p2", "dir":["d"]}
    # ]
    # @]
    # Устройство файла @@mirrors.json@@:
    # *Поле "ftp" — содержит имя FTP-сервера
    # *Поле "usr" — содержит логин
    # *Поле "pwd" — содержит пароль
    # *Поле "dir" — содержит путь к целевой папке в виде списка каталогов
    #
    def ftp_upload_mirror(_mirror_info, _file_name):
        def ftp_src_upload(_ftp, __file_name):
            out_name = os.path.split(__file_name)[1]
            _ftp.storlines(
                "STOR " + out_name, open(__file_name)
            )
        try:
            ftp = ftplib.FTP(_mirror_info["ftp"])
            ftp.login(_mirror_info["usr"], _mirror_info["pwd"])
            for folder_name in _mirror_info["dir"]:
                ftp.cwd(folder_name)
            ftp_src_upload(ftp, _file_name)
        except:
            return traceback.format_exc()

    def process_test():
        # Используем текущий файл для конвертации в pmWiki-файл и выкладывания
        # полученного файла на сервер через FTP
        print("test only")
        file_path = os.path.abspath(os.path.normpath(sys.argv[0]))
        file_path = file_path.replace("\\", "/")

        print(sys.argv[0])
        f = file(sys.argv[0], "r")
        b = f.read()
        w = WikiFileFromPython(b)

        w.save_to_file("Main.TestUpload")
        failed = ftp_update_mirrors("mirrors.json", "Main.TestUpload")

        # Используем свеже отправленый тест для обратного преобразования
        # файла вики в исходный код проекта
        from urllib2 import urlopen
        host = "http://galaxy00.esy.es"
        page = "Main.ResEditorsCodeExample"
        info = urlopen(host+"?n="+page+"&action=source")
        print(info)
        buff = info.read()
        wiki_file = WikiFile(buff)

    def find_rule(path, name):
        rule_dir = path
        last_dir = "?"
        while last_dir:
            list_dir = os.listdir(rule_dir)
            if name in list_dir:
                return os.path.join(rule_dir, name)
            rule_dir, last_dir = os.path.split(rule_dir)
        return None

    def process_main():
        file_path = sys.argv[1]
        if not os.path.exists(file_path):
            raise BaseException("ERROR: file not exists \"%s\"" % file_path)
        if not os.path.isfile(file_path):
            raise BaseException("ERROR: is not some file \"%s\"" % file_path)
        file_dir, file_name = os.path.split(file_path)
        print("file_name = '%s'" % file_name)
        print("cwd       = '%s'" % os.getcwd())
        mirrors_json_file_path = find_rule(file_dir, "mirrors.json")
        if not mirrors_json_file_path:
            raise BaseException("ERROR: no mirrors.json file defined")
        if not os.path.isfile(mirrors_json_file_path):
            raise BaseException("ERROR: mirrors.json is not some file")
        print("mirrors_json_file_path = '%s'" % mirrors_json_file_path)

        with open(file_path, "r") as code_file:
            code_buff = code_file.read()
            wiki_page = WikiFileFromPython(code_buff)
            work_rule_path = find_rule(file_dir, "WORK")
            if not work_rule_path:
                raise BaseException("ERROR:work_rule not found")

            encoded_base_dir = WikiFile.path_to_group(work_rule_path)
            encoded_file_dir = WikiFile.path_to_group(file_dir)
            page_group = encoded_file_dir.split(encoded_base_dir, 1)[-1]
            if page_group.startswith("Dir"):
                page_group = page_group[3:]
            page_group = "Gen" + page_group

            page_title = WikiFile.name_to_title(file_name)

            wiki_page.page_group = page_group
            wiki_page.page_title = page_title

            wiki_file_name = ".".join((page_group, page_title))
            wiki_temp_file_name = os.path.join(file_dir, wiki_file_name)
            print(work_rule_path)
            print(file_dir)
            print(page_group)
            print(page_title)
            print(wiki_file_name)
            print(wiki_temp_file_name)
            print("*-"*12)
            wiki_page.save_to_file(wiki_temp_file_name)

        _failed = []
        with open(mirrors_json_file_path) as mirrors_file:
            mirrors_json = mirrors_file.read()
            mirrors_list = json.loads(mirrors_json)
        mirrors_len = len(mirrors_list)
        print("="*10+"<FTP MIRRORS UPDATE>"+"="*10)
        for mirror in mirrors_list:
            print(mirror)
            fail = ftp_upload_mirror(mirror, wiki_temp_file_name)
            if fail:
                print(fail)
                _failed.append((mirror,fail))
        print("="*10+"<MIRRORS UPDATE END>"+"="*10)
        print("| UPDATED %s FROM %s FTP mirrors" % (
            mirrors_len - len(_failed), mirrors_len
        ))
        return _failed
        print("="*20)


    if 1 == len(sys.argv):
        process_test()
    else:
        process_main()
=======
#!/usr/bin/python
# coding=utf-8
from __future__ import print_function

import os
import re
import json
import time
import ftplib
import traceback

python_code_mark_regexp = re.compile("^\s*#\s?WIKI\.(\S*?):(.*)$")
python_code_part_regexp = re.compile("^\s*#(.*)$")
wiki_python_mark_regexp = re.compile("(^\(:gen_from_python\s*(.*):\))(.*)$")
wiki_page_groups_regexp = re.compile("^\s*(\S*)\s*$")
has_forward_empty_space = re.compile("^(\s\s*)(.*)$")
wiki_cpp_mark_regexp = re.compile("(^\(:gen_from_cpp\s*(.*):\))(.*)$")
is_empty_line_regexp = re.compile("^(\s*)$")

pmWiki_gen_begin = "^(:\s*comment\s+PYTHON.GEN_UPLOADS_HERE_BEGIN\s*:).*$"
pmWiki_gen_end   = "^(:\s*comment\s+PYTHON.GEN_UPLOADS_HERE_END\s*:).*$"

# WIKI.PAGE: gen/src/scr/utils/py2pmWiki.py
# WIKI.HEAD: Данный модуль предназначен для преобразования исходного кода
# языков программирования, используемых в проекте, в исходный код '''pmWiki'''.
# А также для обратного преобразования исходного кода '''pmWiki''' страницы
# в код исходного языка программирования.

# WIKI.HEAD: !!!Мотивация
# *[[Main.DevelopmentConvey|В раздел "Цикл разработки": \
# В чем заключается производственный процесс...]]
# *[[Main.DevelopersWiki|В раздел "Среда разработки": Роль pmWiki в данном \
# проекте...]]
#
# Эта часть конвейера автоматизации существенно улучшает читаемость кода и его
# документации, за счет возможности включения в исходный код программы вики-\
# маркапов для вставки графических схем и гиперссылок на любые интернет
# документы необходимые для понимания устройства данного кода.
#
# (:footnote:)
# Важно знать техническое задание, используемые интерфейсы, умолчания и
# ограничения. Нужно знать координаты автора данного кода, для быстрой связи
# с ним в случае если код перестал компилироваться, работать или обнаружилась
# другая его уязвимость. Получив пароль доступа к соответствующей странице
# проекта, автор сможет немедленно внести поправки в свой код из ближайшего
# интернет кафе, используя редактор встроенный в вики. Описание этого варианта
# правки кода объясняет необходимость реализации преобразования вики текста
# в исходный код языка программирования.
# (:footnoteend:)
#
# Кроме прочего, удаление исходного файла из проекта автоматически удалит и
# размещённую в нём документацию. Это избавляет от работ по ручной
# синхронизации двух параллельных иерархий: кода и документации к коду.
#
# В случае изменения алгоритма внутри исходника, документация, которую нужно
# поправить, расположена рядом — в этом же файле. Это избавляет от поиска
# страниц документации, в которые эти правки нужно внести.
#
# !!!Реализация.
# Реализация затруднена тем, что весь первоначальный загрузчик проекта на
# машину пользователя должен, в идеале, являться единым исходником, который
# и нужно забрать средствами первого попавшегося браузера для разворачивания
# всех остальных исходных файлов уже из вики. Где-то в вики должна быть папка
# с неупакованным в вики исходником, содержащая такой загрузчик. Для запуска
# которого, достаточно проинсталлировать на машину Python.
#
# Для успешной реализации декодера Wiki => src, с доступом через TCP или
# HTTP ( не HTTPS! ), нужно помнить роль VTSS в RTS. А именно то, что члены
# VTSS - автоматы состояний с сильными взаимными связями и высокой частотой
# передачи управления главному диспетчеру событий (требования RTS:—синхронная
# гостиница для синхронных ботов). Декодер Wiki => src не обязательно делать
# синхронным ботом, но такое развитие событий не исключено. Бот VTSS, который
# отслеживает вики-страницу может оказаться полезным элементом системы отладки
# как источник сигнала для быстрой реакции на изменение содержимого страницы.
# Например при отладке шейдеров, сразу после сохранения шейдера бот сможет
# автообновить шейдер на стороне видео, за счет ежесекундного сканирования
# страницы. Но в релизе такое сканирование, максимум ежедневное. То-же
# касается и обновления файлов с геометрией, текстурами и т.п. Само собой, что
# такое решение не верно и избыточо, что нужен механизм посылающий запрос
# каждое сохранение страницы на IP определённого VTSS, к которому подписан
# UISS начинающий по этому сигналу обновление данных на стороне GPU.


# WIKI.TEXT: !!!Класс WikiFile
# Предназначен для преобразования файла из pmWiki-формата в исходный файл на
# Python, C/C++, GLSL, ... в зависимости от наличия вики файле однострочного
# маркапа @@wiki_source@@ и его аргументов
class WikiFile(object):
    """pmWiki object created from pmWiki source file"""
    # Это обычный комментарий не конвертируемый в вики текст
    def __init__(self, wiki_source):
        """Create new WikiFile instance
        :param wiki_source: unicode object with pmWiki page source text
        """
        self.wiki = wiki_source  # pmWiki source text
        self.role = u'wiki'      # wiki|python|c|c++|glsl
        self.code = u''          # data or code extracted from wiki source
        self.page_title = u"Title.ext"
        self.page_group = u"Group"

    def as_wiki_source(self):
        """Real method as_wiki_source must be overridden in derived classes
        :return: target wiki-page's source text
        """
        return self.__class__.as_wiki_source.__doc__

    def page_file_name(self):
        return self.page_group + u"." + self.page_title
# ...
    # WIKI.TEXT: !!!Статические методы WikiFile:
    # *'''@@.path_to_group(путь к папке) =>@@''' ''имя группы в вики''
    # *'''@@.name_to_title(имя файла   ) =>@@''' ''имя страницы в вики''
    # *'''@@.group_to_path(имя группы  ) =>@@''' ''исходный путь к папке''
    # *'''@@.title_to_name(имя страницы) =>@@''' ''исходное имя файла''

# ... class WikiFile
    @staticmethod
    def path_to_group(path):
        g = os.path.abspath(os.path.normpath(path))
        g = g.replace("\\", "/")
        g = "Dir".join((g.split("/")))
        g = g.replace("_", "Sub")
        g = g.replace(".", "Dot")
        return g

    @staticmethod
    def group_to_path(g):
        g = g.replace("Dot", ".")
        g = g.replace("Sub", "_")
        g = "/".join(g.split("Dir"))
        return g

    @staticmethod
    def name_to_title(name):
        t, ext = os.path.splitext(name)
        t = t.replace("_", "Sub")
        t = t.replace(".", "Dot")
        if ext:
            t = "Gen" + t + "Gen" + ext[1:]
        else:
            t = "Gen" + t + "Gen"
        return t

    @staticmethod
    def title_to_name(generated_title):
        name, ext = generated_title.rsplit("Gen", 1)
        name = name.split("Gen", 1)[1]
        name = name.replace("Dot", ".")
        name = name.replace("Sub", "_")
        if ext:
            return ".".join((name, ext))
        return name

# ...
    # WIKI.TEXT: !!!Метод WikiFile.wiki_src_to_wiki_file
    # Сохраняет вики-страницу в файл, пригодный для переноса на pmWiki сайт.
    # Файл будет размещён в текущей рабочей директории.
    # Читать [[http://www.pmwiki.org/wiki/PmWiki/PageFileFormat\
    # |описание формата файла '''pmWiki''']] ...
    # Если имя файла не указано, то используется имя возвращаемое методом
    # '''@@page_file_name@@'''
    # Записывает текущее время в поле '''@@ctime@@''' - "время создания" файла.
    # Записывает имя '''@@"admin"@@''' в поле '''@@author@@''' и имя файла в
    # поле '''@@name@@'''

# ... class WikiFile
    def save_to_file(self, file_name=None):
        if not file_name:
            file_name = self.page_file_name()
        with open(file_name, 'w') as out:
            src = self.as_wiki_source()
            src = src.replace("%", "%25")
            src = src.replace("\n", "%0a")
            out.write("version=pmwiki-2.2.70 ordered=1 urlencoded=1\n")
            out.write("agent=Mozilla/5.0 (X11; U; Linux i686; ")
            out.write("en-US; rv:1.8.1.19) Gecko/20081216 Ubuntu/8.04 (hardy)")
            out.write(" Firefox/2.0.0.19\n")
            out.write("author=admin\n")
            out.write("charset=UTF-8\n")
            #  out.write("csum=mv\n")
            out.write("title=%s\n" % self.page_title)
            out.write("name=%s\n" % self.page_file_name())
            out.write("ctime=%d\n" % int(time.time()))
            out.write("text=")
            out.write(src.encode('utf8'))
            out.write("\n")
            out.close()

    def parse_markups(self, src):
        # Любой вики файл мог быть сгенерирован автоматически из
        # исходного кода на другом языке программирования:
        # wiki_python_mark_regexp
        # wiki_c_mark_regexp
        # wiki_cpp_mark_regexp
        # wiki_glsl_mark_regexp
        pass


# WIKI.TEXT: !!!Класс WikiGenText
# Генерируемый блок текста в вики формате. Предназначен для накопления строк
# вики текста, на промежуточном этапе генерации вики-текста из исходного кода
class WikiGenText(object):
    def __init__(self):
        self.lines = []

    def append(self, line):
        clean = self.clean_line(line)
        if isinstance(clean, str):
            self.lines.append(clean.decode("utf-8"))
        elif isinstance(clean, unicode):
            self.lines.append(clean) 

    def is_empty(self):
        return not len(self.lines)

    def as_wiki_source(self):
        return '\n'.join(self.lines)

    @staticmethod
    def clean_line(line):
        # has_forward_empty_space = re.compile("^(\s\s*)(.*)$")
        match = has_forward_empty_space.match(line)
        if match:
            line = match.groups()[1]
        return line


# WIKI.TEXT: !!!Класс WikiGenCode
# Генерируемый блок исходного кода программы в вики-формате. Предназначен для
# накопления строк фрагмента исходного кода на промежуточном этапе генерации
# вики-текста внутри многострочного маркапа "source"
class WikiGenCode(object):
    def __init__(self):
        self.lines = []
        self.empty_tail_lines_count = 0

    def append(self, line):
        if isinstance(line, str):
            self.lines.append(line.decode("utf-8"))
        elif isinstance(line, unicode):
            self.lines.append(line) 

    def is_empty(self):
        return not len(self.lines)

    def as_wiki_source(self):
        len_lines = len(self.lines)
        for r in (-i-1 for i in xrange(len_lines)):
            match = is_empty_line_regexp.match(self.lines[r])
            if match:
                self.empty_tail_lines_count += 1
            else:
                break
        code_lines = len_lines - self.empty_tail_lines_count
        wiki_lines = u""
        if code_lines:
            head = u'(:sour'+u'ce lang=python -getcode -trim:)[@'
            tail = u'@'+u']\n'
            body = u"\n".join(self.lines[:code_lines])
            wiki_lines = head + body + tail
        wiki_lines += u"\n".join(self.lines[code_lines:])+u"\n"
        return wiki_lines


# WIKI.TEXT: !!!Класс WikiFileFromPython
# Предназначен для создания вики-текста из исходного файла на языке
# программирования '''Python'''
class WikiFileFromPython(WikiFile):
    """pmWiki object created from Python source file"""
    def __init__(self, python_source):
        """Create new WikiFile instance from Python source
        :param python_source: Python source, possible with wiki markups
        """
        WikiFile.__init__(self, u"")
        self.role = u'python'
        self.code = python_source.split('\n')
        self.code_len = len(self.code)
        # Временные переменные для коллекционирования python-markups
        self.head = []               # optional, pre-code wiki source
        self.tail = []               # optional, post-code wiki source
        self.body = [WikiGenCode()]  # each line without markup is a code line
        self.parse_python_markups()

    def as_wiki_source(self):
        result = u""

        result += u"(:comm"+u"ent "+u"parsed head::)\n"
        for i in self.head:
            result += i.as_wiki_source() + u'\n'

        result += u"(:comm"+u"ent "+u"parsed body::)\n"
        for i in self.body:
            result += i.as_wiki_source() + u'\n'

        result += u"(:comm"+u"ent "+u"parsed tail::)\n"
        for i in self.tail:
            result += i.as_wiki_source() + u'\n'

        result += u"(:comm"+u"ent "+u"parsed stop::)\n"
        return result
# ...
    # WIKI.TEXT:!!!Метод WikiFileFromPython.parse_python_markups:
    #
    # Осуществляет поиск маркеров вики-вставок в комментариях к исходному коду
    # Python. Парсер рассматривает исходный текст построчно. Если строка
    # начинается с '''@@'# WIKI.ИМЯ_МЕТОДА:'@@''', при этом лидируюшие пробелы
    # игнорируются, то будет произведена проверка наличия метода с именем
    # '''@@'gen_p2w_имя_метода'@@''' у данной инстанции класса. Если метод
    # имеется в наличии, то парсер вызовет его, иначе — проигнорирует
    # совпадение и будет обрабатывать эту строчку, как очередную строчку
    # исходного кода.\

# ... class WikiFileFromPython
    def parse_python_markups(self):
        line = 0                         # current parsed line number
        while line < self.code_len:
            match = python_code_mark_regexp.match(self.code[line])
            if match:
                parse_method = "gen_p2w_%s" % match.groups()[0].lower()
                if hasattr(self, parse_method):
                    accepted = getattr(self, parse_method)(match, line)
                    if accepted:
                        line += accepted
                        continue
            # No regexp match, or no method, or not accepted?
            # — So, current line is part of last body's code block.
            # If last block is not some code block, create new one:
            curr_block = self.body[-1]
            if not isinstance(curr_block, WikiGenCode):
                curr_block = WikiGenCode()
                self.body.append(curr_block)
            # TODO: append current line to current code block
            curr_block.append(self.code[line])
            line += 1
        # On stop iteration lines.

    # WIKI.TEXT: !!!Метод WikiFileFromPython.gen_p2w_page
    #
    # Парсер однострочной метки '''@@WIKI.PAGE@@''' — наименования файла в
    # базе знаний. Если разбор строки прошел успешно, метод возвращает 1,
    # сообщая о том что принял одну строку текста. Иначе возвращает 0,
    #  тем самым вынуждая парсер обработать строку как обычную строку кода.\

# ... class WikiFileFromPython
    def gen_p2w_page(self, match, line):
        """Parse markup 'WIKI.PAGE' — single line markup with wiki-page name.
        For example: # WIKI.PAGE: PYTHON/utils/Group/py2pmWiki.py
        Generate page name "PythonUtilsGroup.py2pmWiki_gen_py".
        :param match: python_code_mark_regexp.match object with data groups
        :param line: index of current parsed line in source code
        :return count of accepted lines or None if line not accepted
        """
        path_match = wiki_page_groups_regexp.match(match.groups()[1])
        if path_match:
            print("%04d|>>>gen_p2w_page" % line, path_match.groups())
            path = path_match.groups()[0]
            pth = path.split('/')
            self.page_group = str().join([i.lower().title() for i in pth[:-1]])
            self.page_title = pth[-1]  # .rsplit(".", 1)[0]
            print("page_file_name =", self.page_file_name())
            return 1
        # return None is like return 0

    # #WIKI.NAME: Однострочная метка — заголовок страницы в базе знаний.
    def gen_p2w_name(self, match, line):
        print("%04d|>>>gen_p2w_name" % (line + 1), match.groups(), self)
        return 1

    # #WIKI.HEAD: Многострочня метка — содержит параграф вики-страницы,
    # который выносится за пределы кода, и располагается выше кода, но ниже
    # предыдущего параграфа созданного аналогичным тегом.
    def gen_p2w_head(self, match, line):
        print("%04d|>>>gen_p2w_head" % (line + 1), match.groups(), self)
        curr_block = WikiGenText()
        curr_block.append(match.groups()[1])
        self.head.append(curr_block)
        next = line+1
        accepted = 1
        while next <= self.code_len:
            next_match = python_code_part_regexp.match(self.code[next])
            if next_match:
                print("----|>>>gen_p2w_text", next_match.groups()[0])
                curr_block.append(next_match.groups()[0])
                accepted += 1
                next += 1
                continue
            else:
                break
        curr_block.append("\n")
        return accepted

    # WIKI.TAIL: Многострочня метка — содержит параграф вики-страницы,
    # который выносится за пределы исходного кода, и располагается ниже кода
    # и ниже любого предыдущего параграфа созданного аналогичным тегом.
    def gen_p2w_tail(self, match, line):
        print("%04d|>>>gen_p2w_tail" % (line + 1), match.groups(), self)
        return 1

    # WIKI.TEXT: Многострочня метка — содержит параграф вики-страницы,
    # который рассекает текщий блок исходного кода, либо является следующим
    # параграфом в предыдущем рассечении
    def gen_p2w_text(self, match, line):
        curr_block = self.body[-1]
        if isinstance(curr_block, WikiGenCode):
            if curr_block.is_empty():
                # curr block is empty code block
                # TODO: Try switch to prev block "self.body[-2]"
                if len(self.body)>1:
                    curr_block = self.body[-2]
                else:
                    code_head = self.body.pop(-1)
                    curr_block = WikiGenText()
                    self.body.append(curr_block)
                    self.body.append(code_head)
            else:
                # curr block is code block and it is not empty:
                curr_block = WikiGenText()
                self.body.append(curr_block)
        assert(isinstance(curr_block, WikiGenText))
        last_block_line = len(curr_block.lines)
        # TODO: append current line to current code block
        print("%04d|>>>gen_p2w_text\n" % (line+1), self.code[line])
        curr_block.append(u"\n")
        curr_block.append(match.groups()[1])
        next = line+1
        accepted = 1
        while next < self.code_len:
            next_match = python_code_part_regexp.match(self.code[next])
            if next_match:
                print("----|>>>gen_p2w_text", next_match.groups()[0])
                curr_block.append(next_match.groups()[0])
                accepted += 1
                next +=1
                continue
            else:
                break
        return accepted


# WIKI.TEXT: !!!Тесты и примеры использования.
#
# Если модуль используется как основной файл проекта,
# то запускается процесс его тестирования.
#
# Тестовый код содержит и примеры использования.
if __name__ == '__main__':
    import sys

    # WIKI.TEXT:!!!Отправка результирующих файлов на pmWiki сайт проекта
    # Конечно ваш собственный список ftp сайтов и паролей к ним может быть
    # организован по другому, более разумно, но для простоты будем полагать
    # что список ftp сайтов и паролей к ним находится в файле @@mirrors.json@@
    # в текущей рабочей директории (@@current working directory@@).
    # И организован как '''@@JSON@@''' список элементов вида:
    # (:source lang=python -getcode:)[@
    # [{"ftp":"ftp.myWiki0.net", "usr":"name0", "pwd": "p0", "dir":["aa","d"]}
    # ,{"ftp":"a30.pmWiki1.net", "usr":"name1", "pwd": "p1", "dir":["d","d"]}
    # ,{"ftp":"g11.pmWiki2.net", "usr":"name2", "pwd": "p2", "dir":["d"]}
    # ]
    # @]
    # Устройство файла @@mirrors.json@@:
    # *Поле "ftp" — содержит имя FTP-сервера
    # *Поле "usr" — содержит логин
    # *Поле "pwd" — содержит пароль
    # *Поле "dir" — содержит путь к целевой папке в виде списка каталогов
    #
    def ftp_update_mirrors(_file_name_mirrors, _file_name):
        def ftp_src_upload(_ftp, __file_name):
            _ftp.storlines(
                "STOR " + __file_name, open(__file_name)
            )
        # def ftp_bin_upload(_ftp, __file_name):
        #     _ftp.storbinary(
        #         "STOR " + __file_name, open(__file_name, "rb"), 1024
        #     )
        _failed = []
        with open(_file_name_mirrors) as mirrors_file:
            mirrors_json = mirrors_file.read()
            mirrors_list = json.loads(mirrors_json)
        mirrors_len = len(mirrors_list)
        print("="*10+"<FTP MIRRORS UPDATE>"+"="*10)
        for mirror in mirrors_list:
            try:
                print(mirror)
                ftp = ftplib.FTP(mirror["ftp"])
                ftp.login(mirror["usr"], mirror["pwd"])
                for folder_name in mirror["dir"]:
                    ftp.cwd(folder_name)
                ftp_src_upload(ftp, _file_name)
            except:  #TODO: Indicate more concrete upload error
                exc = traceback.format_exc()
                _failed.append((mirror, exc))
        print("="*10+"<MIRRORS UPDATE END>"+"="*10)
        print("| UPDATED %s FROM %s FTP mirrors" % (
            mirrors_len - len(_failed), mirrors_len
        ))
        return _failed

    # WIKI.TEXT:!!!Отправка результирующих файлов на pmWiki сайт проекта
    # Конечно ваш собственный список ftp сайтов и паролей к ним может быть
    # организован по другому, более разумно, но для простоты будем полагать
    # что список ftp сайтов и паролей к ним находится в файле @@mirrors.json@@
    # в текущей рабочей директории (@@current working directory@@).
    # И организован как '''@@JSON@@''' список элементов вида:
    # (:source lang=python -getcode:)[@
    # [{"ftp":"ftp.myWiki0.net", "usr":"name0", "pwd": "p0", "dir":["aa","d"]}
    # ,{"ftp":"a30.pmWiki1.net", "usr":"name1", "pwd": "p1", "dir":["d","d"]}
    # ,{"ftp":"g11.pmWiki2.net", "usr":"name2", "pwd": "p2", "dir":["d"]}
    # ]
    # @]
    # Устройство файла @@mirrors.json@@:
    # *Поле "ftp" — содержит имя FTP-сервера
    # *Поле "usr" — содержит логин
    # *Поле "pwd" — содержит пароль
    # *Поле "dir" — содержит путь к целевой папке в виде списка каталогов
    #
    def ftp_upload_mirror(_mirror_info, _file_name):
        def ftp_src_upload(_ftp, __file_name):
            out_name = os.path.split(__file_name)[1]
            _ftp.storlines(
                "STOR " + out_name, open(__file_name)
            )
        try:
            ftp = ftplib.FTP(_mirror_info["ftp"])
            ftp.login(_mirror_info["usr"], _mirror_info["pwd"])
            for folder_name in _mirror_info["dir"]:
                ftp.cwd(folder_name)
            ftp_src_upload(ftp, _file_name)
        except:
            return traceback.format_exc()

    def process_test():
        # Используем текущий файл для конвертации в pmWiki-файл и выкладывания
        # полученного файла на сервер через FTP
        print("test only")
        file_path = os.path.abspath(os.path.normpath(sys.argv[0]))
        file_path = file_path.replace("\\", "/")

        print(sys.argv[0])
        f = file(sys.argv[0], "r")
        b = f.read()
        w = WikiFileFromPython(b)

        w.save_to_file("Main.TestUpload")
        failed = ftp_update_mirrors("mirrors.json", "Main.TestUpload")

        # Используем свеже отправленый тест для обратного преобразования
        # файла вики в исходный код проекта
        from urllib2 import urlopen
        host = "http://galaxy00.esy.es"
        page = "Main.ResEditorsCodeExample"
        info = urlopen(host+"?n="+page+"&action=source")
        print(info)
        buff = info.read()
        wiki_file = WikiFile(buff)

    def find_rule(path, name):
        rule_dir = path
        last_dir = "?"
        while last_dir:
            list_dir = os.listdir(rule_dir)
            if name in list_dir:
                return os.path.join(rule_dir, name)
            rule_dir, last_dir = os.path.split(rule_dir)
        return None

    def process_main():
        file_path = sys.argv[1]
        if not os.path.exists(file_path):
            raise BaseException("ERROR: file not exists \"%s\"" % file_path)
        if not os.path.isfile(file_path):
            raise BaseException("ERROR: is not some file \"%s\"" % file_path)
        file_dir, file_name = os.path.split(file_path)
        print("file_name = '%s'" % file_name)
        print("cwd       = '%s'" % os.getcwd())
        mirrors_json_file_path = find_rule(file_dir, "mirrors.json")
        if not mirrors_json_file_path:
            raise BaseException("ERROR: no mirrors.json file defined")
        if not os.path.isfile(mirrors_json_file_path):
            raise BaseException("ERROR: mirrors.json is not some file")
        print("mirrors_json_file_path = '%s'" % mirrors_json_file_path)

        with open(file_path, "r") as code_file:
            code_buff = code_file.read()
            wiki_page = WikiFileFromPython(code_buff)
            work_rule_path = find_rule(file_dir, "WORK")
            if not work_rule_path:
                raise BaseException("ERROR:work_rule not found")

            encoded_base_dir = WikiFile.path_to_group(work_rule_path)
            encoded_file_dir = WikiFile.path_to_group(file_dir)
            page_group = encoded_file_dir.split(encoded_base_dir, 1)[-1]
            if page_group.startswith("Dir"):
                page_group = page_group[3:]
            page_group = "Gen" + page_group

            page_title = WikiFile.name_to_title(file_name)

            wiki_page.page_group = page_group
            wiki_page.page_title = page_title

            wiki_file_name = ".".join((page_group, page_title))
            wiki_temp_file_name = os.path.join(file_dir, wiki_file_name)
            print(work_rule_path)
            print(file_dir)
            print(page_group)
            print(page_title)
            print(wiki_file_name)
            print(wiki_temp_file_name)
            print("*-"*12)
            wiki_page.save_to_file(wiki_temp_file_name)

        _failed = []
        with open(mirrors_json_file_path) as mirrors_file:
            mirrors_json = mirrors_file.read()
            mirrors_list = json.loads(mirrors_json)
        mirrors_len = len(mirrors_list)
        print("="*10+"<FTP MIRRORS UPDATE>"+"="*10)
        for mirror in mirrors_list:
            print(mirror)
            fail = ftp_upload_mirror(mirror, wiki_temp_file_name)
            if fail:
                print(fail)
                _failed.append((mirror,fail))
        print("="*10+"<MIRRORS UPDATE END>"+"="*10)
        print("| UPDATED %s FROM %s FTP mirrors" % (
            mirrors_len - len(_failed), mirrors_len
        ))
        return _failed
        print("="*20)


    if 1 == len(sys.argv):
        process_test()
    else:
        process_main()
>>>>>>> e80704d2f7f95a5beb6e1e6d387ed2eca1824182
