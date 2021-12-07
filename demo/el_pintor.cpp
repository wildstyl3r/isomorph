#include "QGrypho.hpp"
#include "isomorph.hpp"

#include <QApplication>
#include <QFileDialog>
#include <iostream>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QGrypho qg, gv;
  QVector<View> g_view_cache;
  QVector<QGrypho*> g_qg_cache;
  auto f = QFileDialog::getOpenFileName(&qg,
                                        "Open Graph",
                                        "~/",
                                        "Graph Files (*.tgf *.dot)").toStdString();
  if (f != "") {
      Graph g(f);

      for(auto& c : g.colors()){
          c = 0;
      }
      ClassificationReport cr = classify_canonical_ext(g);
      g_view_cache.clear();
      for(auto &qg : g_qg_cache){
          delete qg;
      }
      g_qg_cache.clear();
      for(vertex v = 0; v < g.V().size(); ++v){
          g_view_cache.push_back(View(g, v));
          View& view = g_view_cache.back();
          g_qg_cache.push_back(new QGrypho);
          g_qg_cache.back()->setWindowTitle(QString::fromStdString(view.id(v)+" ("+std::to_string(view.color(v))+")"));
      }

      qg.drawGraph(&g);
      qg.text = "recoloring iterations: " + QString::number(cr.iterations) + "\n" +
              "time: " + QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(cr.time).count()) + "ms\n";
      qg.setDblClick([&](int v, bool left){
          if (g.has(v)){
              if (left){
                  if(!g_qg_cache[v]->isGraphSet()){
                      g_qg_cache[v]->drawGraph(g_view_cache[v].lexmst_tree());
                      g_qg_cache[v]->highlight(QVector<vertex>(1,v));
                  }
                  g_qg_cache[v]->show();
              }
          }
      });
      qg.setSelect([&](int v){
          if (g.has(v)){
              QVector<edge> es;
              if(!qg.highlighted(v)){
                  qg.highlight(QVector<vertex>(1,v));
                  for(vertex i = 0; i < g_view_cache[v].V().size(); ++i){
                      for(vertex j = 0; j < g_view_cache[v].V().size(); ++j){
                          if (g_view_cache[v].has({i,j})){
                              es.push_back(edge({i,j}));
                          }
                      }
                  }
                  qg.highlight(es);
              } else {
                  for(edge& e : g_view_cache[v].lexmst_edges()){
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
