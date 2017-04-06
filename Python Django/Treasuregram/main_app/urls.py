from django.conf.urls import url
from .import views
from django.views.static import serve
from django.conf import settings

urlpatterns=[
    url(r'^$',views.index),
    url(r'^([0-9]+)/$',views.detail,name='detail'),
    url(r'^post_url/$',views.post_treasure,name='post_treasure'),
    url(r'^user/(\w+)/$', views.profile, name='profile'),
    url(r'^login/$', views.login_view,name='login'),
    url(r'^logout/$', views.logout_view,name='logout')

]

if settings.DEBUG:
    urlpatterns+=[
        url(r'^media/(?P<path>.*)$',serve,{'document_root': settings.MEDIA_ROOT})
    ]

