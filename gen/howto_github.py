#!/usr/bin/python
# -*- coding: utf-8 -*-

# Инсталлируем модуль github: > pip install pygithub
# Фаза первая. Пишем настолько примитивно, насколько это вообще возможно.

from __future__ import print_function
from json import loads, dumps
from pprint import pprint
from github import*

# Читаем файл с паролями и настройками спрятанный на локальной машине
try:
    git_conf_file = open("E:/web/gitpwd.json", "r").read()
except IOError:
    git_conf_file = '''
    {"password"  : null
    ,"username"  : "andreytata"
    }
    '''

git_conf = loads(git_conf_file)
username = git_conf["username"]
password = git_conf["password"]

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

exp57 = None
for repo in user.get_repos():
    print(repo.name)
    if repo.name == "exp57":
        exp57 = repo
        break

if None is exp57:
    print("Repository exp57 a not found for user '%s'" % username)
    sys.exit()

# print(help(exp57.get_dir_contents))
for content_file in exp57.get_dir_contents(""):
    print("%-12s"%content_file.raw_data['name'], content_file.raw_data['url'])
    print(len(content_file.raw_data))
    print(content_file.type)

print(type(content_file))
print(help(content_file))
