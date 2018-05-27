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


class Project:
    def __init__(self, name, repository):
        self.username, self.repository = name, repository
        self.tree = dict()
        self.curr = []
        print("++%s, %s" % (self.username, self.repository.name))

    def explore(self):
        """Build url's tree, using GitHub API, for 'self.repository'
        :return: None
        """
        print("\n[%s]" % self.repository.full_name)
        for ctx in self.repository.get_dir_contents(""):
            print("%-16s %-8s %s" % (ctx.name, ctx.type, ctx.url))
            subj = json.loads(urlopen(ctx.url).read())
            # В этом месте отображаем тип ctx.type на вызов соответствующего
            # метода, что должно выбросить исключение, в ситуации, когда нет
            # метода для обработки объекта неизвестного, на этом этапе, типа:
            # ctx.type со значением "Xx", вызовет исключение AttributeError:
            # AttributeError: Project instance has no attribute 'explore_Xx'
            apply(getattr(self, "explore_" + ctx.type), [subj])

    def explore_file(self, ctx):
        """Collect values from file info
        :param ctx: 'dict' with 'GitHub API' file info
        :return: None
        """
        name, sha, url = ctx["name"], ctx["sha"], ctx["url"]
        print(name, sha, url)

    def explore_dir(self, ctx):
        """Collect values from folder info
        :param ctx: 'list' with 'GitHub API v3' folder info
        :return: None
        """
        print("contain %s records" % len(ctx))


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
