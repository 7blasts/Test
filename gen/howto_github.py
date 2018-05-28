#!/usr/bin/python
# -*- coding: utf-8 -*-

# Инсталлируем модуль github: > pip install pygithub
# Фаза первая. Пишем настолько примитивно, насколько это вообще возможно.

from __future__ import print_function
import json
from github import*
try:
    from urllib2 import urlopen
except:  # python3 ?
    from urllib.request import urlopen


class Folder:
    def __init__(self, name, parent=None):
        self.name, self.parent = name, parent
        self.folders, self.files = dict(), dict()
        self.level = 0
        while parent:
            self.level += 1
            parent = parent.parent

    def add_git_hub_dir(self, subj):
        """ Add folder item from github
        :param subj: 'dict' with parsed GitHub API v3 JSON folder info
        :return: None
        """
        sub_folder_name = subj["name"]
        print("----" * self.level, 'd', subj, self)
        print("    " * self.level + '->', sub_folder_name)
        sub_folder = Folder(sub_folder_name, self)
        self.folders[sub_folder_name] = sub_folder

        #apply(getattr(sub_folder, "add_git_hub_"+node["type"]), [node])

    def add_git_hub_file(self, subj):
        """ Add source item from github
        :param subj: 'dict' with parsed GitHub API v3 JSON file info
        :return: None
        """
        print(self.level, 'f', subj, self)

    def is_root(self):
        return None is self.parent

    def line_info(self):
        return str([self.name, "dir", len(self.folders), len(self.files)])


class Project(Folder):
    def __init__(self, name, repository):
        Folder.__init__(self, name, None)
        self.username, self.repository = name, repository
        print("++%s, %s" % (self.username, self.repository.name))

    def explore(self):
        """Build url's tree, using GitHub API, for 'self.repository'
        :return: None
        """
        print("\n[%s]" % self.repository.full_name)
        for ctx in self.repository.get_dir_contents(""):
            print("%-16s %-8s %s" % (ctx.name, ctx.type, ctx.url))
            info = json.loads(urlopen(ctx.url).read())
            # В этом месте отображаем тип ctx.type на вызов соответствующего
            # метода, что должно выбросить исключение, в ситуации, когда нет
            # метода для обработки объекта неизвестного, на этом этапе, типа:
            # ctx.type со значением "Xx", вызовет исключение AttributeError:
            # AttributeError: Project instance has no attribute 'explore_Xx'
            apply(getattr(self, "project_" + ctx.type), (ctx, info))

        print("-*"*30)
        folder_names = self.folders.keys()
        folder_names.sort()
        for folder_name in folder_names:
            print("[ %-12s ]" % folder_name, self.folders[folder_name].line_info())

        source_names = self.files.keys()
        source_names.sort()
        for source_name in source_names:
            print("  %-12s  " % source_name, self.files[source_name])

    def project_file(self, ctx, info):
        """Collect values from file info
        :param ctx:
        :param info: 'dict' with 'GitHub API' file item info
        :return: None
        """
        print(ctx.name, ctx, info, self)
        self.files[ctx.name] = info  # ctx.name, ctx.sha, ctx.url ...

    def project_dir(self, ctx, info):
        """Collect values from folder info
        :param ctx:
        :param info: 'list' with 'GitHub API v3' folder item info
        :return: None
        """
        print(ctx.name, ctx, info)
        folder = Folder(ctx.name, self)
        self.folders[ctx.name] = folder
        for node in info:
            apply(getattr(folder, "add_git_hub_"+node["type"]), [node])

# Читаем файл с паролями и настройками спрятанный на локальной машине
try:
    git_conf_file = open("C:/web/gitpwd.json", "r").read()
except IOError:
    git_conf_file = '''
    {"username"  : "andreytata"
    ,"projects" : [ "exp57", "plugins" ]
    ,"password"  : null
    }
    '''

git_conf = json.loads(git_conf_file)
username = git_conf["username"]  # строка
password = git_conf["password"]  # строка или None
projects = git_conf["projects"]  # LIST OF PROJECTS

# Создаём инстанцию класса Github используя логин и пароль
print(git_conf)
print("Login:%s" % [username, password])
if password:
    gh = Github(username, password)
    user = gh.get_user()
else:
    gh = Github()
    user = gh.get_user(username)

print(gh.get_api_status())

projects_list = []

for r in user.get_repos():
    print("exists '%s'" % r.full_name)
    if r.name in projects:
        projects_list.append(Project(username, r))

for p in projects_list:
    p.explore()

if len(projects_list) < len(projects):
    explored = [i.repository.name for i in projects_list]
    projects_lost = [i for i in projects if i not in explored]
    for lost in projects_lost:
        print("WARNING: '%s' has no repository '%s'" % (username, i))
