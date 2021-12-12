#include "QGrypho.hpp"
#include "isomorph.hpp"

#include <QApplication>
#include <QFileDialog>
#include <iostream>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QGrypho qg, gv;
  //QVector<View> g_view_cache;
  QVector<QGrypho*> g_qg_cache;
  auto f = QFileDialog::getOpenFileName(&qg,
                                        "Open Graph",
                                        "~/",
                                        "Graph Files (*.tgf *.dot *.cgf)").toStdString();
  if (f != "") {
      Graph g(f);

      for(auto& c : g.colors()){
          c = 0;
      }
      ClassificationReport cr(g);
      //g_view_cache.clear();
      for(auto &qg : g_qg_cache){
          delete qg;
      }
      g_qg_cache.clear();
      for(vertex v = 0; v < g.V().size(); ++v){
          //g_view_cache.push_back(View(g, v));
          //View& view = g_view_cache.back();
          g_qg_cache.push_back(new QGrypho);
          g_qg_cache.back()->setWindowTitle(QString::fromStdString(cr.views()[v]->id(v)+" ("+std::to_string(cr.views()[v]->color(v))+")"));
      }

      qg.drawGraph(&g);
      qg.text = "recoloring iterations: " + QString::number(cr.iterations()) + "\n" +
              "time: " + QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(cr.time()).count()) + "ms\n";
      qg.setDblClick([&](Graph* w, int v, bool left){
          if (w->has(v)){
              if (left){
                  if(!g_qg_cache[v]->isGraphSet()){
                      g_qg_cache[v]->drawGraph(cr.views()[v]->lexmst_tree());
                      g_qg_cache[v]->highlight(QVector<vertex>(1,v));
                  }
                  g_qg_cache[v]->show();
              }
          }
      });
      qg.setSelect([&](Graph* w, int v){
          if (w->has(v)){
              QVector<edge> es;
              if(!qg.highlighted(v)){
                  qg.highlight(QVector<vertex>(1,v));
                  for(vertex i = 0; i < cr.views()[v]->V().size(); ++i){
                      for(vertex j = 0; j < cr.views()[v]->V().size(); ++j){
                          if (cr.views()[v]->has({i,j})){
                              es.push_back(edge({i,j}));
                          }
                      }
                  }
                  qg.highlight(es);
              } else {
                  for(edge& e : cr.views()[v]->lexmst_edges()){
                      es.push_back(edge(e));
                  }
                  qg.highlight(es);
              }
          } else {
              qg.highlight(QVector<vertex>());
              qg.highlight(QVector<edge>());
          }
      });
      qg.show();
      return a.exec();
  } else {
      return 0;
  }
}
