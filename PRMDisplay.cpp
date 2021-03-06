
#include "PRMDisplay.h"
#include "RBN.h"
#include <Shapes.h>
#include <sstream>


using namespace LibBoard;
using namespace prm;

PRMDisplay::PRMDisplay(const boost::shared_ptr<prm::RBN> rbn, const Graph& graph):rbn(rbn), graph(graph){
	board.clear( Color(255,255,255) );
	board.setUnit(1, Board::UCentimeter);
	board.setPenColorRGBi( 255, 0, 0);
	board.setLineWidth( 1 );
	srand (time(NULL));
	delta=0.01;
	
}
PRMDisplay::PRMDisplay(const boost::shared_ptr<prm::RBN> rbn, const PositionMap& postionMap, const VertexIndexPropertyMap& vertexNamemap, const Graph& graph):rbn(rbn), positionMap(postionMap), vertexIdPropertyMap(vertexNamemap), graph(graph){
	this->board.clear( Color(255,255,255) );
	board.setUnit(1, Board::UCentimeter);
  board.setPenColorRGBi( 255, 0, 0);
  board << LibBoard::Rectangle( -8, 12, 16, 24, Color::Black, Color::None, 0.1 );
  board.setLineWidth( 2 );
  srand (time(NULL));
}

PRMDisplay::PRMDisplay(const PositionMap& postionMap, const VertexIndexPropertyMap& vertexNamemap, const Graph& graph):positionMap(postionMap), vertexIdPropertyMap(vertexNamemap), graph(graph){
	this->board.clear( Color(255,255,255) );
	board.setUnit(1, Board::UCentimeter);
  board.setPenColorRGBi( 255, 0, 0);
  board.setLineWidth( 2 );
  srand (time(NULL));
}

float  random_gray() {
  return rand() / 255;
}

void PRMDisplay::placeVertex(){
	std::map<std::string, Color> colorclasse; 
	int nbclasse=5;
	srand( static_cast<unsigned int>( time(0) ) );
	boost::graph_traits<Graph>::vertex_iterator i, end;
    for (boost::tie(i, end) = boost::vertices(graph); i != end; ++i) {
        std::cout << "ID: (" << vertexIdPropertyMap[*i] << ") x: " << positionMap[*i][0] << " y: " << positionMap[*i][1] << "\n";
		std::string classename=vertexIdPropertyMap[*i].substr(0,vertexIdPropertyMap[*i].find("."));
		// couleur
		makeVertexColor(classename);
		board.drawText(positionMap[*i][0]+.1,positionMap[*i][1]+0.4, vertexIdPropertyMap[*i] );
    }
	placeClasse();

}

void PRMDisplay::makeVertexColor(const std::string& classename){
	bool trouve=false;
		if (listColor.empty()){
			ColorClass c;
			c.classeName=classename;
				c.red=rand() / 255;
				c.blue=rand() / 255;
				c.green=rand() / 255;
			listColor.push_back(c);
			board.setPenColorRGBi(c.red, c.green, c.blue);
		}else{

			
			for (std::vector<ColorClass>::iterator it = listColor.begin();it != listColor.end(); ++it){
				if (it->classeName.compare(classename)==0){
				 trouve=true;
				 board.setPenColorRGBi(it->red, it->green, it->blue);
				 break;
				}
			}
			if(!trouve){
				ColorClass c;
				c.classeName=classename;
				c.red=rand() / 255;
				c.blue=rand() / 255;
				c.green=rand() / 255;
				listColor.push_back(c);
				board.setPenColorRGBi(c.red, c.green, c.blue);
			}
		}
}

void PRMDisplay::placeClasse(){
	boost::graph_traits<Graph>::vertex_iterator i, end;
    for (boost::tie(i, end) = boost::vertices(graph); i != end; ++i) {
		std::string classename=vertexIdPropertyMap[*i].substr(0,vertexIdPropertyMap[*i].find("."));
		bool trouve=false;
		if (listRect.empty()){
			RectClass c;
			c.classeName=classename;
			c.x=positionMap[*i][0];
			c.y=positionMap[*i][1];
			c.len=3;
			c.hei=.5;
			listRect.push_back(c);
			
		}else{		
			for (std::vector<RectClass>::iterator it = listRect.begin();it != listRect.end(); ++it){
				if (it->classeName.compare(classename)==0){
					trouve=true;
					// je regle la longueur du rectangle
					if ((it->x<=positionMap[*i][0]) && ((it->x)+(it->len)>=(positionMap[*i][0]+3))) {

					}else if (it->x>positionMap[*i][0]){
						it->len+=std::abs(it->x-positionMap[*i][0]);
					}else if ((it->x)+(it->len)<(positionMap[*i][0]+3)){
						it->len+=std::abs(positionMap[*i][0]+3-(it->x+it->len));
					}
					// je regle la hauteur du rectangle
					if ((it->y<=positionMap[*i][1]) && ((it->y)+(it->hei)>=(positionMap[*i][1]+.5))) {

					}else if (it->y>positionMap[*i][1]){
						it->hei+=std::abs(it->y-positionMap[*i][1]);
					}else if ((it->y)+(it->hei)<(positionMap[*i][1]+.5)){
						it->hei+=std::abs(positionMap[*i][1]+.5-(it->y+it->hei));
					}
					//je regle le point en haut � gauche du rectangle
					if (it->x>positionMap[*i][0]) { it->x=positionMap[*i][0];}
					if (it->y>positionMap[*i][1]) { it->y=positionMap[*i][1];}
								
				 break;
				}
			}
			
			if(!trouve){
				std::cout<<"Classe: "<<classename<<std::endl;
				RectClass c;
				c.classeName=classename;
				c.x=positionMap[*i][0];
				c.y=positionMap[*i][1];
				c.len=3;
				c.hei=.5;
				listRect.push_back(c);
			}
		}
	}
	// je dessine les rectangles de classe avec la bonne couleur de pinceau
	for (std::vector<RectClass>::iterator it = listRect.begin();it != listRect.end(); ++it){
		for (std::vector<ColorClass>::iterator it1 = listColor.begin();it1 != listColor.end(); ++it1){
				if (it1->classeName.compare(it->classeName)==0){
				 board.setPenColorRGBi(it1->red, it1->green, it1->blue);
				 break;
				}
			}
		board.drawRectangle(it->x-delta,it->y-delta,it->len+delta,it->hei+delta);
	}
}

void PRMDisplay::placeRelationnalLink(){
	board.setPenColorRGBi( 255, 0, 0);
	
	std::map<std::string, std::string> result; // model (classe.FK, classe1.PK)
	RefSlotsMultimap refSlots = rbn->getSchema().getRefSlots();
	// Fisrt: get all the link between PK and FK	
	for(RefSlotsMultimap::iterator rb = refSlots.begin(); rb != refSlots.end(); ++rb){
		std::string classPK=rb->second.second->getName().c_str();
		std::string classFK=rb->first.c_str();;
		classFK.append(".");
		classFK.append(rb->second.first.getName());
		result.insert(std::pair<std::string, std::string>(classFK, classPK));
	}
	// Second: display the links
	boost::graph_traits<Graph>::vertex_iterator i, end;
	for (std::map<std::string,std::string>::iterator it=result.begin(); it!=result.end(); ++it){
		for (boost::tie(i, end) = boost::vertices(graph); i != end; ++i) {
			if (vertexIdPropertyMap[*i].compare(it->first)==0) {
				for (std::vector<RectClass>::iterator it1 = listRect.begin();it1 != listRect.end(); ++it1){
					if (it->second.compare(it1->classeName)==0) {
						float x1,x2,y1,y2;
							x1=positionMap[*i][0];
							y1=positionMap[*i][1];
							x2=it1->x;
							y2=it1->y;
							// Comparer les position pour placer x et Y correctement pour l'attribut et pour la classe ref�rente
							if (positionMap[*i][1]<it1->y){// le noeud 1 est plus haut que noeud 2
								y1+=.5;
							}else {
								y2+=it1->hei;
							}
							if (positionMap[*i][0]<it1->x){// le noeud 1 est plus droite que noeud 2
								x1+=3;
							}else {
								x2+=it1->len;
							}
						board.drawLine(x1,y1,x2,y2);
						break;
					}
				}
				break;
			}
		}
	}
				
}

void PRMDisplay::placeProbabilistLink(){ 
	board.setPenColorRGBi( 0, 0,255);
	boost::graph_traits<Graph>::vertex_iterator i, end;
	RBNVariablesSequence seq;
	 for (boost::tie(i, end) = boost::vertices(graph); i != end; i++) {
		
		if (rbn->existsNode(vertexIdPropertyMap[*i])){
			seq=rbn->getParents(vertexIdPropertyMap[*i]);
			if (seq.dim()>0) {
				for(unsigned int j = 0; j < seq.dim(); j++){
					boost::graph_traits<Graph>::vertex_iterator i1, end1;
					for (boost::tie(i1, end1) = boost::vertices(graph); i1 != end1; ++i1) {
						if (vertexIdPropertyMap[*i1].compare(dynamic_pointer_cast<IRBNSimpleVariable>(seq[j])->getBaseName())==0){
							float x1,x2,y1,y2;
							x1=positionMap[*i1][0];
							y1=positionMap[*i1][1];
							x2=positionMap[*i][0];
							y2=positionMap[*i][1];
							// Comparer les position pour placer x et Y correctement
							if (positionMap[*i1][1]<positionMap[*i][1]){// le noeud 1 est plus haut que noeud 2
								y1+=.5;
							}else {
								y2+=.5;
							}
							if (positionMap[*i1][0]<positionMap[*i][0]){// le noeud 1 est plus droite que noeud 2
								x1+=3;
							}else {
								x2+=3;
							}
							board.drawArrow(x1,y1, x2,y2);
							break;
						}
					}
				}
			}
		}
	 }
			
	
}

void PRMDisplay::display(const std::string& path, const std::string& name){
	std::string completepath;
	completepath.append(path);
	completepath.append("\\");
	completepath.append(name);
	//completepath.append(".svg");
	board.saveSVG(completepath.append(".svg").c_str(),4000,4000);
	//board.saveFIG(completepath.append(".fig").c_str(),Board::BoundingBox, 10.0);
}

void PRMDisplay::RBNToGraph(const double attributeWeight, const double FKWeight) {
	Class aClass;
	VertexDescriptor vd;
	std::map<std::string, VertexDescriptor> container;
	positionMap = boost::get(&VertexProperties::point, graph);
	vertexIdPropertyMap = boost::get(&VertexProperties::index, graph);
	WeightPropertyMap weightPropertyMap = boost::get(&EdgeProperty::weight, graph);
	RelationalSchema schema = rbn->getSchema();
	std::vector<std::string> classnames = schema.getClassNames();
	for(std::vector<std::string>::iterator it = classnames.begin(); it != classnames.end(); ++it)
	{
		std::string classname = *it;

		aClass = schema.getClass(classname);
		std::vector<std::string> attributesName = aClass.getAttributeNames();
		//std::vector<std::string>::iterator it2 = attributesName.begin();

		//pour chaque attribut, ajout d'un nouveau sommet aux coordonn�e (1,1)
		for(std::vector<std::string>::iterator it1 = attributesName.begin(); it1!=attributesName.end(); it1++)
		{
			std::stringstream ss;
			ss << classname << "." << *it1;

			vd = boost::add_vertex(graph);

			vertexIdPropertyMap[vd] = ss.str();
			container[ss.str()] = vd;
			positionMap[vd][0]=1;
			positionMap[vd][1]=1;
			// si l'it�rateur est diff�rent de begin, alors on itere pour ajouter une ar�te jusqu'� l'it�rateur it1 actuel
			if(attributesName.begin() != it1)
			{
				//Ajout d'ar�te entre le nouveau sommet et ceux du graphe qui appartiennent � la m�me classe. Le poid est de 2
				for(std::vector<std::string>::iterator it3 = attributesName.begin(); it3 != it1; it3++)
				{
					std::stringstream ss2;
					ss2 << classname << "." << *it3;
					boost::add_edge(container[ss.str()], container[ss2.str()], EdgeProperty(attributeWeight), graph);
				}
			}
		} 
	}
	
	/***** ajout d'ar�te pour chaque cl� �trang�re ****/
	RefSlotsMultimap foreignKeys = schema.getRefSlots();
	std::string attPK;
	std::string classPK;
	std::string classFK;
	for(RefSlotsMultimap::iterator rb = foreignKeys.begin(); rb != foreignKeys.end(); ++rb){
		classPK=rb->second.second->getName().c_str();
		attPK= rbn->getSchema().getClass(classPK).getPK().front();
		classPK.append(".");
		classPK.append(attPK);
		//std::cout << "\t primaryKey "<<classPK;
		classFK=rb->first.c_str();;
		classFK.append(".");
		classFK.append(rb->second.first.getName());
		//std::cout << "\t foriegnKey "<<classFK;
		boost::add_edge(container[classFK], container[classPK], EdgeProperty(FKWeight), graph);
	}
}

void PRMDisplay::usedKamada(const double sidelength){
	boost::minstd_rand gen;
	Topology topology(gen, 50.0);

	WeightPropertyMap weightPropertyMap = boost::get(&EdgeProperty::weight, graph);
	boost::circle_graph_layout(graph, positionMap, sidelength);
	
	bool result = boost::kamada_kawai_spring_layout(graph, 
                                       positionMap,
                                       weightPropertyMap,
									   topology,
                                       side_length(sidelength),
                                       kamada_kawai_done());
	
	displayKamadaCheck(result);
	adjustDisplayAfterKamada(sidelength);

}

void PRMDisplay::adjustDisplayAfterKamada(const double length){
	boost::graph_traits<Graph>::vertex_iterator i, end;
	for (boost::tie(i, end) = boost::vertices(graph); i != end; ++i) {
		positionMap[*i][0] += length;
		positionMap[*i][1] += length;
	}
}

void PRMDisplay::displayKamadaCheck(bool kamadaResult){
	if (kamadaResult)
		std::cout << "now displaying"<<std::endl;
	else
		std::cerr << "kamada_kawai_spring_layout failed" << std::endl;
}

void PRMDisplay::RBNToGraph_AllAttributsConnected(const double attributeWeight, const double FKWeight, const double probWeight){
	/*rechercher toutes classes et tous les attributs*/
	std::map<std::string, VertexDescriptor> verticeContainer;
	RelationalSchema schema = rbn->getSchema();
	std::vector<std::string> classnames = schema.getClassNames();
	std::vector<std::string> attributnames;
	std::string verticeName, secondVertice;
	
	for(std::vector<std::string>::iterator classsNameIterator = classnames.begin(); classsNameIterator != classnames.end(); ++classsNameIterator){
		attributnames = schema.getClass(*classsNameIterator).getAttributeNames();

		for(std::vector<std::string>::iterator attributNameIterator = attributnames.begin(); attributNameIterator != attributnames.end(); ++attributNameIterator ){
			verticeName = *classsNameIterator;
			verticeName.append(".");
			verticeName.append(*attributNameIterator);
			addVertex(verticeName, verticeContainer);
			
			if(attributNameIterator != attributnames.begin()){
				for(std::vector<std::string>::iterator it = attributnames.begin(); it != attributNameIterator; ++it){
					secondVertice = *classsNameIterator;
					secondVertice.append(".");
					secondVertice.append(*it);
					boost::add_edge(verticeContainer[verticeName], verticeContainer[secondVertice], EdgeProperty(attributeWeight), graph);
				}
			}
		}
	}
	addForeignKeyEdges_multiConnectedAttributs(verticeContainer, FKWeight);
	addProbabilistLink(verticeContainer, probWeight);
}

void PRMDisplay::addVertex(const std::string& verticeName, std::map<std::string, VertexDescriptor>& container){
	VertexDescriptor vd;
	vertexIdPropertyMap = boost::get(&VertexProperties::index, graph);
	positionMap = boost::get(&VertexProperties::point, graph);

	vd = boost::add_vertex(graph);
	vertexIdPropertyMap[vd] = verticeName;
	positionMap[vd][0]=1;
	positionMap[vd][1]=1;
	container[verticeName] = vd;
}

void PRMDisplay::addForeignKeyEdges_multiConnectedAttributs(std::map<std::string, VertexDescriptor>& verticeContainer, const double edgeWweight){
	/***** ajout d'ar�te pour chaque cl� �trang�re ****/
	RefSlotsMultimap foreignKeys = rbn->getSchema().getRefSlots();
	std::string classPK, classFK;
	std::vector<std::string> classPKAttributs, classFKAttributes;

	for(RefSlotsMultimap::iterator refSlotIterator = foreignKeys.begin(); refSlotIterator != foreignKeys.end(); ++refSlotIterator){
		classFK = refSlotIterator->first;
		classPK = refSlotIterator->second.second->getName();
		std::vector<std::string> PKClassAttributs = rbn->getSchema().getClass(classPK).getAttributeNames();
		std::vector<std::string> FKClassAttributs = rbn->getSchema().getClass(classFK).getAttributeNames();

		for(std::vector<std::string>::iterator PKAttributsIterator = PKClassAttributs.begin();
		  PKAttributsIterator != PKClassAttributs.end();
		  ++PKAttributsIterator){
			for(std::vector<std::string>::iterator FKAttributsIterator = FKClassAttributs.begin();
			  FKAttributsIterator != FKClassAttributs.end();
			  ++FKAttributsIterator){
				  std::stringstream vertice1, vertice2;
				  vertice1 << classPK << "." << *PKAttributsIterator;
				  vertice2 << classFK << "." << *FKAttributsIterator;
				  boost::add_edge(verticeContainer[vertice1.str()], verticeContainer[vertice2.str()], EdgeProperty(edgeWweight), graph);
			}
		}
		
		
	}
}

void PRMDisplay::addProbabilistLink(std::map<std::string, VertexDescriptor>& verticeContainer, const double edgeWweight){
	std::map<std::string, VertexDescriptor>::iterator verticeIterator;
	std::string vertexName, parentName;
	prm::RBNVariablesSequence variablesSequence;


	for(verticeIterator = verticeContainer.begin(); verticeIterator != verticeContainer.end(); verticeIterator++){
		vertexName = verticeIterator->first;
		
		if(rbn->existsNode(vertexName)){
			variablesSequence = rbn->getParents(vertexName);
			for(unsigned int i = 0; i < variablesSequence.dim(); i++){
				parentName = variablesSequence[i]->toString();
				boost::add_edge(verticeContainer[vertexName], verticeContainer[parentName], EdgeProperty(edgeWweight), graph);
			}
		}
	}
}