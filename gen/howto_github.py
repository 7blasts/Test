#!/usr/bin/python
# -*- coding: utf-8 -*-

# Инсталлируем модуль github: > pip install pygithub
# Фаза первая. Пишем настолько примитивно, насколько это вообще возможно.

from __future__ import print_function
from json import loads, dumps
from pprint import pprint
from github import*
import sys

# Читаем файл с паролями и настройками спрятанный на локальной машине
# Файл JSON примерно следующего содержания
sample_json = \
'''
{"password"  : "user_password"
,"user"      : "user_name"
}
'''
gitpwd_json = file("C:/web/gitpwd.json","r").read()
gitpwd = loads(gitpwd_json)
username = gitpwd["user"]
password = gitpwd["password"]

# Создаём инстанцию класса Github используя логин и пароль
print("Hello Git")
pprint(gitpwd)
gh = Github(username, password)
print(gh.get_api_status())

user = gh.get_user()

exp57 = None
for repo in user.get_repos():
    print(repo.name)
    if repo.name == "exp57":
        exp57 = repo

    ##repo.edit(has_wiki=False)

if None == exp57:
    sys.exit()

print(help(exp57.get_dir_contents))
for content_file in exp57.get_dir_contents(""):
    print (content_file.raw_data)
    sys.exit()


sys.exit()
print(help(gh))

pprint(gh.users(username).get())  # Это должно выдать JSON, что то вроде:
'''
{'avatar_url': u'https://avatars3.githubusercontent.com/u/20459584?v=4',
 'bio': None,
 'blog': u'',
 'collaborators': 0,
 'company': None,
 'created_at': u'2016-07-14T13:30:29Z',
 'disk_usage': 98,
 'email': None,
 'events_url': u'https://api.github.com/users/andreytata/events{/privacy}',
 'followers': 1,
 'followers_url': u'https://api.github.com/users/andreytata/followers',
 'following': 0,
 'following_url': u'https://api.github.com/users/andreytata/following{/other_user}',
 'gists_url': u'https://api.github.com/users/andreytata/gists{/gist_id}',
 'gravatar_id': u'',
 'hireable': None,
 'html_url': u'https://github.com/andreytata',
 'id': 20459584,
 'location': None,
 'login': u'andreytata',
 'name': None,
 'organizations_url': u'https://api.github.com/users/andreytata/orgs',
 'owned_private_repos': 0,
 'plan': {'collaborators': 0,
          'name': u'free',
          'private_repos': 0,
          'space': 976562499},
 'private_gists': 0,
 'public_gists': 0,
 'public_repos': 2,
 'received_events_url': u'https://api.github.com/users/andreytata/received_events',
 'repos_url': u'https://api.github.com/users/andreytata/repos',
 'site_admin': False,
 'starred_url': u'https://api.github.com/users/andreytata/starred{/owner}{/repo}',
 'subscriptions_url': u'https://api.github.com/users/andreytata/subscriptions',
 'total_private_repos': 0,
 'two_factor_authentication': False,
 'type': u'User',
 'updated_at': u'2016-07-14T13:30:30Z',
 'url': u'https://api.github.com/users/andreytata'}
'''
repo = gh.get_user().get_repo("exp57")
pprint(repo.get_file_contents())
# /repos/:owner/:repo/git/trees/:tree_sha

#pprint(help(repo))

#https://raw.githubusercontent.com/7blasts/Test/master/gen/howto_github